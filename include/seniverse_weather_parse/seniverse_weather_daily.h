#ifndef __SENIVERSE_WEATHER_DAILY_H__
#define __SENIVERSE_WEATHER_DAILY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "seniverse_weather_def.h"

#define WEATHER_DAILY_SUMMARY_MAX_LEN      16
#define WEATHER_DAILY_UPDATE_TIME_MAX_LEN  WEATHER_UPDATE_TIME_MAX_LEN

struct weather_daily_item {
    char date[WEATHER_DAILY_SUMMARY_MAX_LEN];
    char text_day[WEATHER_DAILY_SUMMARY_MAX_LEN];
    char text_night[WEATHER_DAILY_SUMMARY_MAX_LEN];
    char wind_direction[WEATHER_DAILY_SUMMARY_MAX_LEN];
    float high_temp;
    float low_temp;
    float precip;
    float wind_direction_degree;
    float wind_speed;
    float wind_scale;
    float rainfall;
    float humidity;
    int8_t code_day;
    int8_t code_night;
};

struct weather_daily {
    struct seniverse_weather_common common;
    struct weather_daily_item items[];
};

int parse_weather_daily(const char *buf, struct weather_daily *daily, int *count);
int weather_daily_get_url_api(char *url, int url_max_len, char *key, char *location, enum SENIVERSE_LANGUAGE_TYPE language, enum SENIVERSE_UNIT_TYPE unit, int start, int count);
int dump_weather_daily(const struct weather_daily *daily);

#ifdef __cplusplus
}
#endif
#endif