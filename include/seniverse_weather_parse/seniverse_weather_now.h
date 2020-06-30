#ifndef __SENIVERSE_WEATHER_NOW_H__
#define __SENIVERSE_WEATHER_NOW_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "seniverse_weather_def.h"

#define WEATHER_NOW_SUMMARY_MAX_LEN      16
#define WEATHER_NOW_UPDATE_TIME_MAX_LEN  WEATHER_UPDATE_TIME_MAX_LEN

struct weather_now_item {
    char summary[WEATHER_NOW_SUMMARY_MAX_LEN];
    char wind_direction[WEATHER_NOW_SUMMARY_MAX_LEN];
    float temperature;
    float feels_like;
    float pressure;
    float visibility;
    float wind_speed;
    float wind_scale;
    float dew_point;
    float wind_direction_degree;
    float clouds;
    float humidity;
    int8_t code;
};

struct weather_now {
    struct seniverse_weather_common common;
    struct weather_now_item items[];
};

int parse_weather_now(const char *buf, struct weather_now *now);
int weather_now_get_url_api(char *url, int url_max_len, char *key, char *location, enum SENIVERSE_LANGUAGE_TYPE language, enum SENIVERSE_UNIT_TYPE unit);
int dump_weather_now(const struct weather_now *now);

#ifdef __cplusplus
}
#endif
#endif