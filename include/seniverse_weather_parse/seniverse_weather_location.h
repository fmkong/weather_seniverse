#ifndef __SENIVERSE_WEATHER_LOCATION_H__
#define __SENIVERSE_WEATHER_LOCATION_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "cJSON.h"

#define WEATHER_LOCATION_MAX_LEN      32

struct weather_location {
    char id[WEATHER_LOCATION_MAX_LEN];
    char name[WEATHER_LOCATION_MAX_LEN];
    char country[WEATHER_LOCATION_MAX_LEN];
    char path[WEATHER_LOCATION_MAX_LEN];
    char timezone[WEATHER_LOCATION_MAX_LEN];
    char timezone_offset[WEATHER_LOCATION_MAX_LEN];
};

int parse_weather_location(cJSON *location_obj, struct weather_location *location);

#ifdef __cplusplus
}
#endif

#endif