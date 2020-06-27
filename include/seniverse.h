#ifndef SENIVERSE_H
#define SENIVERSE_H

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "lwip/apps/sntp.h"
#include "lwip/dns.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include "esp_tls.h"

#include "cJSON.h"

#if CONFIG_LANGUAGE_CHINESE_SIMPLIFIED
    #define LANGUAGE "zh-Hans"
#elif CONFIG_LANGUAGE_CHINESE_TRADITIONAL
    #define LANGUAGE "zh-Hans"
#elif CONFIG_LANGUAGE_ENGLISH
    #define LANGUAGE "zh-Hans"
#else
    #error CONFIG_LANGUAGE is not supported
#endif

#if CONFIG_UNIT_METRIC
    #define UNIT "c"
#elif CONFIG_UNIT_BRITISH_SYSTEM
    #define UNIT "f"
#else
    #error CONFIG_UNIT is not supported
#endif

#define WEB_SERVER "api.seniverse.com"
#define WEB_PORT   "80"
#define WEB_URL    "https://api.seniverse.com/v3/weather/now.json?key=" CONFIG_SENIVERSE_API_KEY "&location=" CONFIG_PLACE_NAME "&language=" LANGUAGE "&unit=" UNIT
//CONFIG_SENIVERSE_API_KEY "/" CONFIG_LATITUDE "," CONFIG_LONGITUDE "?lang=en&exclude=hourly,flag,minutely&units=auto"

static const char* request = "GET " WEB_URL " HTTP/1.1\r\n"
                             "Host: " WEB_SERVER "\r\n"
                             "User-Agent: esp-idf/1.0 esp32\r\n"
                             "\r\n";

/* Root cert for api.seniverse.com, taken from server_root_cert.pem
   The PEM file was extracted from the output of this command:
   openssl s_client -showcerts -connect api.seniverse.com:443 </dev/null
   The CA root cert is the last cert given in the chain of certs.
   To embed it in the app binary, the PEM file is named
   in the component.mk COMPONENT_EMBED_TXTFILES variable.
*/
extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[] asm("_binary_server_root_cert_pem_end");

typedef struct Forecasts {
    time_t time;
    char summary[50];
    char icon[20];
    double temperatureMax;
    double temperatureMin;
    double humidity;
    int pressure;
} Forecast;

char summary[50];
char icon[20];
double temperature;
double humidity;
int pressure;
double wind_speed;
double wind_bearing;
double precip_probability;

Forecast forecasts[8];

const char* deg_to_compass(int degrees);
void get_current_weather_task(void* pvParameters);

#endif // SENIVERSE_H