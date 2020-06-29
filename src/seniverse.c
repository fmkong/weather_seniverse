#include "seniverse.h"
#include "seniverse_weather.h"

#define MAX_URL_BUFFER_LEN (256)
#define MAX_RESP_BUFFER_LEN  (1024*10)

#if CONFIG_LANGUAGE_CHINESE_SIMPLIFIED
    #define LANGUAGE SENIVERSE_LANGUAGE_CHINESE_SIMP
#elif CONFIG_LANGUAGE_CHINESE_TRADITIONAL
    #define LANGUAGE SENIVERSE_LANGUAGE_CHINESE_TRAD
#elif CONFIG_LANGUAGE_ENGLISH
    #define LANGUAGE SENIVERSE_LANGUAGE_ENGLISH
#else
    #error CONFIG_LANGUAGE is not supported
#endif

#if CONFIG_UNIT_METRIC
    #define UNIT SENIVERSE_UNIT_METRIC
#elif CONFIG_UNIT_BRITISH_SYSTEM
    #define UNIT SENIVERSE_UNIT_BRITISH
#else
    #error CONFIG_UNIT is not supported
#endif

#define WEB_SERVER "api.seniverse.com"

static const char* TAG = "seniverse";
/* Root cert for api.seniverse.com, taken from server_root_cert.pem
   The PEM file was extracted from the output of this command:
   openssl s_client -showcerts -connect api.seniverse.com:443 </dev/null
   The CA root cert is the last cert given in the chain of certs.
   To embed it in the app binary, the PEM file is named
   in the component.mk COMPONENT_EMBED_TXTFILES variable.
*/
extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[] asm("_binary_server_root_cert_pem_end");

static void https_get(const char* url, const char* request, char* buf)
{
    int ret, len;

    /* Wait for the callback to set the CONNECTED_BIT in the event group. */
    // xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to url: %s, request: %s", url, request);
    esp_tls_cfg_t cfg = {
        .cacert_pem_buf = server_root_cert_pem_start,
        .cacert_pem_bytes = server_root_cert_pem_end - server_root_cert_pem_start,
        .skip_common_name = true,
    };

    ESP_LOGI(TAG, "CA: %s, len %d", cfg.cacert_pem_buf, cfg.cacert_bytes);
    struct esp_tls* tls = esp_tls_conn_http_new(url, &cfg);

    if (tls != NULL) {
        ESP_LOGI(TAG, "Connection established...");
    } else {
        ESP_LOGE(TAG, "Connection failed...");
        goto exit;
    }

    size_t written_bytes = 0;
    do {
        ret = esp_tls_conn_write(tls, request + written_bytes, strlen(request) - written_bytes);
        if (ret >= 0) {
            ESP_LOGI(TAG, "%d bytes written", ret);
            written_bytes += ret;
        } else if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(TAG, "esp_tls_conn_write  returned 0x%x", ret);
            goto exit;
        }
    } while (written_bytes < strlen(request));

    ESP_LOGI(TAG, "Reading HTTP response...");

    int offset = 0;

    do {
        char tmp_buf[64];

        len = sizeof(tmp_buf) - 1;
        bzero(tmp_buf, sizeof(tmp_buf));
        ret = esp_tls_conn_read(tls, (char*)tmp_buf, len);

        if (ret == MBEDTLS_ERR_SSL_WANT_WRITE || ret == MBEDTLS_ERR_SSL_WANT_READ)
            continue;

        if (ret < 0) {
            ESP_LOGE(TAG, "esp_tls_conn_read  returned -0x%x", -ret);
            break;
        }

        if (ret == 0) {
            ESP_LOGI(TAG, "connection closed");
            break;
        }

        len = ret;
        ESP_LOGD(TAG, "%d bytes read", len);

        for (int i = 0; i < len; i++) {
            buf[i + offset] = tmp_buf[i];
        }
        offset += len;

    } while (1);

    buf[offset] = 0;

exit:
    if (tls)
        esp_tls_conn_delete(tls);
}

int seniverse_get_weather(enum SENIVERSE_WEATHER_DATA_TYPE type, char *location,
                          void *data, int *count, int start, int cnt)
{
    char web_url[MAX_URL_BUFFER_LEN] = {0};
    char web_req[MAX_URL_BUFFER_LEN] = {0};
    char resp_buf[MAX_RESP_BUFFER_LEN] = {0};

    int url_len = seniverse_get_url_api(type, web_url, MAX_URL_BUFFER_LEN,
                          CONFIG_SENIVERSE_API_KEY, location,
                          LANGUAGE, UNIT, start, cnt);

    if (url_len > MAX_URL_BUFFER_LEN - 1 || url_len < 0) {
        ESP_LOGE(TAG, "web url buffer is not long enough: %d, max %d", url_len,
                 MAX_URL_BUFFER_LEN);
        return -1;
    }

    int req_len = snprintf(web_req, MAX_URL_BUFFER_LEN,
                            "GET %s HTTP/1.1\r\n"
                            "Host: %s\r\n"
                            "User-Agent: esp-idf/1.0 esp32\r\n"
                            "\r\n", web_url, WEB_SERVER);
    if (req_len > MAX_URL_BUFFER_LEN - 1 || req_len < 0) {
        ESP_LOGE(TAG, "web req buffer is not long enough: %d, max %d", req_len,
                 MAX_URL_BUFFER_LEN);
        return -1;
    }

    https_get(web_url, web_req, resp_buf);
    return seniverse_parse_resp(type, resp_buf, data, count);
}