
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cJSON.h"
#include "seniverse_weather_hourly.h"
#include "seniverse_cJson_utils.h"
#include "seniverse_weather.h"

#define LOG_TAG "seniverse_hourly"

static int parse_weather_hourly_item_object(cJSON *json_hourly_item, struct weather_hourly_item *hourly_item)
{
    // Get the hourly weather data from the hourly item object
    if (json_hourly_item == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "json_hourly_item is null");
        return -1;
    }

    parse_object_string_value(json_hourly_item, "time", hourly_item->time, WEATHER_HOURLY_UPDATE_TIME_MAX_LEN);
    parse_object_string_value(json_hourly_item, "text", hourly_item->text, WEATHER_HOURLY_SUMMARY_MAX_LEN);
    hourly_item->code = parse_object_string_to_int(json_hourly_item, "code");
    hourly_item->temperature = parse_object_string_to_float(json_hourly_item, "temperature");
    parse_object_string_value(json_hourly_item, "wind_direction", hourly_item->wind_direction, WEATHER_HOURLY_SUMMARY_MAX_LEN);
    hourly_item->wind_speed = parse_object_string_to_float(json_hourly_item, "wind_speed");
    hourly_item->humidity = parse_object_string_to_float(json_hourly_item, "humidity");

    return 0;
}

static int parse_weather_hourly_array(cJSON *json_hourly_array, struct weather_hourly *hourly, int *count)
{
    // Get the current weather data from the hourly array object
    if (json_hourly_array == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "json_hourly_array is null");
        return -1;
    }
    int array_size = get_object_array_size(json_hourly_array);
    SENIVERSE_LOGD(LOG_TAG, "json_hourly_array size is %d", array_size);
    if (hourly->common.count < array_size) {
        SENIVERSE_LOGE(LOG_TAG, "hourly items buffer is not enough: count %d, array size %d", hourly->common.count, array_size);
    }
    for (int index = 0; index < (hourly->common.count < array_size ? hourly->common.count : array_size); index++) {
        parse_weather_hourly_item_object(get_object_with_index_in_array(json_hourly_array, index), &hourly->items[index]);
    }
    *count = hourly->common.count < array_size ? hourly->common.count : array_size;
    return 0;
}

int parse_weather_hourly(const char *buf, struct weather_hourly *hourly, int *count)
{
    // Data can be found after the HTTP header, get te offset to get the data
    char *data_offset = strstr(buf, "\r\n\r\n");

    SENIVERSE_LOGD(LOG_TAG, "get resp: %s", data_offset);

    cJSON *json = cJSON_Parse(data_offset);
    if (json == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "Error in cJSON_Parse: [%s]", IS_NULL(cJSON_GetErrorPtr()));
        return -1;
    }

    cJSON *json_results = cJSON_GetObjectItemCaseSensitive(json, "results");
    if (json_results == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "Error in get item result: [%s]", IS_NULL(cJSON_GetErrorPtr()));
    }

    cJSON *json_location = get_object_with_key_in_array(json_results, "location");
    if (json_location == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "Error in get item location: [%s]", IS_NULL(cJSON_GetErrorPtr()));
    }
    parse_weather_location(json_location, &hourly->common.location);

    cJSON *json_hourly = get_object_with_key_in_array(json_results, "hourly");
    if (json_hourly)
        parse_weather_hourly_array(json_hourly, hourly, count);

    cJSON *json_last_update = get_object_with_key_in_array(json_results, "last_update");
    if (json_last_update && cJSON_IsString(json_last_update) && (json_last_update->valuestring != NULL))
            snprintf(hourly->common.last_update, WEATHER_HOURLY_UPDATE_TIME_MAX_LEN, "%s", json_last_update->valuestring);

    if (json)
        cJSON_Delete(json);
    return 0;
}

int weather_hourly_get_url_api(char *url, int url_max_len, char *key, char *location, enum SENIVERSE_LANGUAGE_TYPE language, enum SENIVERSE_UNIT_TYPE unit, int start, int count)
{
    return snprintf(url, url_max_len, "weather/hourly.json?key=%s&location=%s&language=%s&unit=%s&start=%d&hours=%d", key, location, seniverse_languages[language], seniverse_units[unit], start, count);
}

static int dump_weather_hourly_item(const struct weather_hourly_item *hourly_item)
{
    SENIVERSE_LOGD(LOG_TAG, "\t time: %s", hourly_item->time);
    SENIVERSE_LOGD(LOG_TAG, "\t text: %s", hourly_item->text);
    SENIVERSE_LOGD(LOG_TAG, "\t code: %d", hourly_item->code);
    SENIVERSE_LOGD(LOG_TAG, "\t wind_direction: %s", hourly_item->wind_direction);
    SENIVERSE_LOGD(LOG_TAG, "\t wind_speed: %f", hourly_item->wind_speed);
    SENIVERSE_LOGD(LOG_TAG, "\t temperature: %f", hourly_item->temperature);
    SENIVERSE_LOGD(LOG_TAG, "\t humidity: %f", hourly_item->humidity);
    return 0;
}

int dump_weather_hourly(const struct weather_hourly *hourly)
{
    SENIVERSE_LOGD(LOG_TAG, "Weather hourly: %s", hourly->common.last_update);
    for (int index = 0; index < hourly->common.count; index++) {
        SENIVERSE_LOGD(LOG_TAG, "index %d ===========================", index);
        dump_weather_hourly_item(&hourly->items[index]);
    }
    return 0;
}
