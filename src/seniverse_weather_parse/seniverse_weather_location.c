#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "seniverse_weather_location.h"
#include "seniverse_cJson_utils.h"

#define LOG_TAG "seniverse_location"


int parse_weather_location(cJSON *location_obj, struct weather_location *location)
{
    if (location_obj == NULL) {
        SENIVERSE_LOGE(LOG_TAG, "location_obj is null");
        return -1;
    }
    parse_object_string_value(location_obj, "id", location->id, WEATHER_LOCATION_MAX_LEN);
    parse_object_string_value(location_obj, "name", location->name, WEATHER_LOCATION_MAX_LEN);
    parse_object_string_value(location_obj, "country", location->country, WEATHER_LOCATION_MAX_LEN);
    parse_object_string_value(location_obj, "path", location->path, WEATHER_LOCATION_MAX_LEN);
    parse_object_string_value(location_obj, "timezone", location->timezone, WEATHER_LOCATION_MAX_LEN);
    parse_object_string_value(location_obj, "timezone_offset", location->timezone_offset, WEATHER_LOCATION_MAX_LEN);

    return 0;
}

int dump_weather_location(const struct weather_location *location)
{
    SENIVERSE_LOGD(LOG_TAG, "location:");
    SENIVERSE_LOGD(LOG_TAG, "\t id: %s", location->id);
    SENIVERSE_LOGD(LOG_TAG, "\t name: %s", location->name);
    SENIVERSE_LOGD(LOG_TAG, "\t country: %s", location->country);
    SENIVERSE_LOGD(LOG_TAG, "\t path: %s", location->path);
    SENIVERSE_LOGD(LOG_TAG, "\t timezone: %s", location->timezone);
    SENIVERSE_LOGD(LOG_TAG, "\t timezone_offset: %s", location->timezone_offset);
    return 0;
}