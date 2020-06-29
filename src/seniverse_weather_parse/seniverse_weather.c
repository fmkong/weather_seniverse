#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "seniverse_weather.h"
#include "seniverse_cJson_utils.h"

#define LOG_TAG "seniverse_weather"

#define URL_SENIVERSE "https://api.seniverse.com/v3/"
struct seniverse_weather_obj *creat_weather_data(enum SENIVERSE_WEATHER_DATA_TYPE type, int count)
{
    int item_size;
    switch(type) {
        case SENIVERSE_WEATHER_NOW:
            item_size = sizeof(struct weather_now_item);
            break;
        case SENIVERSE_WEATHER_DAILY:
            item_size = sizeof(struct weather_daily_item);
            break;
        case SENIVERSE_WEATHER_HOURLY:
            item_size = sizeof(struct weather_hourly_item);
            break;
        default:
            item_size = 0;
            break;
    }
    if (item_size == 0) {
        SENIVERSE_LOGE(LOG_TAG, "Item size is not correct.");
        return NULL;
    }
    struct seniverse_weather_obj *obj = SENIVERSE_MALLOC(sizeof(struct seniverse_weather_obj) + item_size * count);
    if (!obj) {
        SENIVERSE_LOGE(LOG_TAG, "Failed to alloc mem for data.");
        return obj;
    }
    obj->common.count = count;
    obj->common.index = type;
    memset(obj->common.last_update, 0, WEATHER_UPDATE_TIME_MAX_LEN);
    memset(obj->items, 0, item_size * count);

    return obj;
}

void destroy_weather_data(struct seniverse_weather_obj *data)
{
    if (data)
        SENIVERSE_FREE(data);
}

int seniverse_get_url_api(enum SENIVERSE_WEATHER_DATA_TYPE type, char *url, int url_max_len, char *key, char *location, enum SENIVERSE_LANGUAGE_TYPE language, enum SENIVERSE_UNIT_TYPE unit, int start, int count)
{
    int ret = 0;
    if (!url) {
        SENIVERSE_LOGE(LOG_TAG, "url buffer is null.");
        return -1;
    }

    ret += snprintf(url, url_max_len, "%s", URL_SENIVERSE);

    switch(type) {
        case SENIVERSE_WEATHER_NOW:
            ret += weather_now_get_url_api(url + ret, url_max_len - ret, key, location, language, unit);
            break;
        case SENIVERSE_WEATHER_DAILY:
            ret += weather_daily_get_url_api(url + ret, url_max_len - ret, key, location, language, unit, start, count);
            break;
        case SENIVERSE_WEATHER_HOURLY:
            ret += weather_hourly_get_url_api(url + ret, url_max_len - ret, key, location, language, unit, start, count);
            break;
        default:
            SENIVERSE_LOGE(LOG_TAG, "Unknown type of response.");
            ret = -1;
            break;
    }

    return ret;
}

int seniverse_parse_resp(enum SENIVERSE_WEATHER_DATA_TYPE type, const char *buf, void *data, int *count)
{
    int ret = 0;
    switch(type) {
        case SENIVERSE_WEATHER_NOW:
            ret = parse_weather_now(buf, (struct weather_now *)data);
            *count = 1;
            break;
        case SENIVERSE_WEATHER_DAILY:
            ret = parse_weather_daily(buf, (struct weather_daily *)data, count);
            break;
        case SENIVERSE_WEATHER_HOURLY:
            ret = parse_weather_hourly(buf, (struct weather_hourly *)data, count);
            break;
        default:
            SENIVERSE_LOGE(LOG_TAG, "Unknown type of response.");
            ret = -1;
            break;
    }

    return ret;
}