#ifndef __SENIVERSE_CJSON_H__
#define __SENIVERSE_CJSON_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "cJSON.h"

#define BUILD_FOR_ESP32 1

#if (BUILD_FOR_ESP32)
#include "esp_log.h"
#endif

#define DEBUG_ENABLE 1


#if (!BUILD_FOR_ESP32)
    #define SENIVERSE_LOGE(TAG, fmt, ...) do{\
        printf("E: %s: " fmt "\n", TAG, ##__VA_ARGS__);\
    } while(0)
    #define SENIVERSE_LOGI(TAG, fmt, ...) do{\
        printf("I: %s: " fmt "\n", TAG, ##__VA_ARGS__);\
    } while(0)
    #define SENIVERSE_LOGD(TAG, fmt, ...)  do{\
        if (DEBUG_ENABLE)\
            printf("D: %s: " fmt "\n", TAG, ##__VA_ARGS__);\
    } while(0)
    #define SENIVERSE_MALLOC(size) malloc(size)
    #define SENIVERSE_FREE(p) free(p)
#else

    #define SENIVERSE_LOGE ESP_LOGE
    #define SENIVERSE_LOGI ESP_LOGI
    #define SENIVERSE_LOGD ESP_LOGD
    #define SENIVERSE_MALLOC(size) malloc(size)
    #define SENIVERSE_FREE(p) free(p)
#endif

#define MAX_STRING_OBJ_BUFFER_LEN 32
#define IS_NULL(x)  ((x) == NULL ? "NULL" : (x))


extern const char *seniverse_languages[];
extern const char *seniverse_units[];

int parse_object_string_value(cJSON *obj, char *key, char *value, int len);
int parse_object_string_to_int(cJSON *obj, char *key);
float parse_object_string_to_float(cJSON *obj, char *key);
cJSON *get_object_with_key_in_array(cJSON *array, char *key);
int get_object_array_size(cJSON *array);
cJSON *get_object_with_index_in_array(cJSON *array, int index);

#ifdef __cplusplus
}
#endif

#endif