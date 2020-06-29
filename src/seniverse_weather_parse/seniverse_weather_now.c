#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cJSON.h"
#include "seniverse_weather_now.h"
#include "seniverse_cJson_utils.h"
#include "seniverse_weather.h"

#define LOG_TAG "seniverse_now"

static int parse_weather_now_object(cJSON *json_now, struct weather_now_item *now)
{
    // Get the current weather data from the now object
    if (json_now == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "json_now is null");
        return -1;
    }

    parse_object_string_value(json_now, "text", now->summary, WEATHER_NOW_SUMMARY_MAX_LEN);
    now->code = parse_object_string_to_int(json_now, "code");
    now->temperature = parse_object_string_to_float(json_now, "temperature");
    now->feels_like = parse_object_string_to_float(json_now, "feels_like");
    now->pressure = parse_object_string_to_float(json_now, "pressure");
    now->humidity = parse_object_string_to_float(json_now, "humidity");
    now->visibility = parse_object_string_to_float(json_now, "visibility");
    parse_object_string_value(json_now, "wind_direction", now->wind_direction, WEATHER_NOW_SUMMARY_MAX_LEN);
    now->wind_direction_degree = parse_object_string_to_float(json_now, "wind_direction_degree");
    now->wind_speed = parse_object_string_to_float(json_now, "wind_speed");
    now->wind_scale = parse_object_string_to_float(json_now, "wind_scale");
    now->clouds = parse_object_string_to_float(json_now, "clouds");
    now->dew_point = parse_object_string_to_float(json_now, "dew_point");
    return 0;
}

int parse_weather_now(const char *buf, struct weather_now *now)
{
    // Data can be found after the HTTP header, get te offset to get the data
    char *data_offset = strstr(buf, "\r\n\r\n");

    SENIVERSE_LOGD(LOG_TAG, "get resp: %s", data_offset);

    cJSON *json = cJSON_Parse(data_offset);
    if (json == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "Error in cJSON_Parse: [%s]", cJSON_GetErrorPtr());
        return -1;
    }

    cJSON *json_results = cJSON_GetObjectItemCaseSensitive(json, "results");
    if (json_results == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "Error in get item result: [%s]", cJSON_GetErrorPtr());
    }

    cJSON *json_location = get_object_with_key_in_array(json_results, "location");
    if (json_location == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "Error in get item location: [%s]", cJSON_GetErrorPtr());
    }
    parse_weather_location(json_location, &now->common.location);

    cJSON *json_now = get_object_with_key_in_array(json_results, "now");
    if (json_now)
        parse_weather_now_object(json_now, now->items);

    cJSON *json_last_update = get_object_with_key_in_array(json_results, "last_update");
    if (json_last_update && cJSON_IsString(json_last_update) && (json_last_update->valuestring != NULL))
            snprintf(now->common.last_update, WEATHER_NOW_UPDATE_TIME_MAX_LEN, "%s", json_last_update->valuestring);

    if (json)
        cJSON_Delete(json);
    return 0;
}

int weather_now_get_url_api(char *url, int url_max_len, char *key, char *location, enum SENIVERSE_LANGUAGE_TYPE language, enum SENIVERSE_UNIT_TYPE unit)
{
    return snprintf(url, url_max_len, "weather/now.json?key=%s&location=%s&language=%s&unit=%s", key, location, seniverse_languages[language], seniverse_units[unit]);
}