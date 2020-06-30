#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "seniverse_weather.h"
#include "seniverse_cJson_utils.h"

#define LOG_TAG "seniverse_status"

int parse_seniverse_status(const char *buf)
{
    // Data can be found after the HTTP header, get te offset to get the data
    char *data_offset = strstr(buf, "\r\n\r\n");

    SENIVERSE_LOGD(LOG_TAG, "get resp: %s", data_offset);

    cJSON *json = cJSON_Parse(data_offset);
    if (json == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "Error in cJSON_Parse: [%s]", IS_NULL(cJSON_GetErrorPtr()));
        return -1;
    }

    cJSON *json_status = cJSON_GetObjectItemCaseSensitive(json, "status");
    if (json_status == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "Error in get item status: [%s]", IS_NULL(cJSON_GetErrorPtr()));
    }
    if (cJSON_IsString(json_status) && (json_status->valuestring != NULL)) {
        SENIVERSE_LOGE(LOG_TAG, "status: [%s]", json_status->valuestring);
    }

    cJSON *json_status_code = cJSON_GetObjectItemCaseSensitive(json, "status_code");
    if (json_status_code == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "Error in get item status_code: [%s]", IS_NULL(cJSON_GetErrorPtr()));
    }
    if (cJSON_IsString(json_status_code) && (json_status_code->valuestring != NULL)) {
        SENIVERSE_LOGE(LOG_TAG, "status_code: [%s]", json_status_code->valuestring);
    }

    if (json)
        cJSON_Delete(json);
    return 0;
}