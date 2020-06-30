#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "cJSON.h"
#include "seniverse_cJson_utils.h"

#define LOG_TAG "seniverse_utils"

const char *seniverse_languages[] = {
    "zh-Hans",  // 简体中文
    "zh-Hant",  // 繁体中文
    "en",       // 英文
    "ja",       // 日语
    "de",       // 德语
    "fr",       // 法语
    "es",       // 西班牙语
};

const char *seniverse_units[] = {
    "c",  // c	当参数为 c 时，温度 c、风速 km/h、能见度 km、气压 mb
    "f",  // f	当参数为 f 时，温度 f、风速 mph、能见度 mile、气压 inch
};

int parse_object_string_value(cJSON *obj, char *key, char *value, int len)
{
    cJSON *obj_item = cJSON_GetObjectItemCaseSensitive(obj, key);
    if (cJSON_IsString(obj_item) && (obj_item->valuestring != NULL)) {
        if (value)
            snprintf(value, len, "%s", obj_item->valuestring);
    } else {
        SENIVERSE_LOGD(LOG_TAG, "Error in get item %s: [%s]", key, IS_NULL(cJSON_GetErrorPtr()));
        return -1;
    }
    return 0;
}

int parse_object_string_to_int(cJSON *obj, char *key)
{
    char buffer[MAX_STRING_OBJ_BUFFER_LEN];
    int ret = 0;

    if (!parse_object_string_value(obj, key, buffer, MAX_STRING_OBJ_BUFFER_LEN))
        ret = atoi(buffer);
    return ret;
}

float parse_object_string_to_float(cJSON *obj, char *key)
{
    char buffer[MAX_STRING_OBJ_BUFFER_LEN];
    float ret = 0;
    if (!parse_object_string_value(obj, key, buffer, MAX_STRING_OBJ_BUFFER_LEN))
        ret = atof(buffer);
    return ret;
}

cJSON *get_object_with_key_in_array(cJSON *array, char *key)
{
    cJSON *result = NULL;
    int array_size = cJSON_GetArraySize(array);
    SENIVERSE_LOGD(LOG_TAG, "array item nums: %d", array_size);

    for(int cnt = 0; cnt < array_size; cnt++) {
        cJSON *array_sub = cJSON_GetArrayItem(array, cnt);
        if(array_sub == NULL) {
            continue;
        }
        if((result = cJSON_GetObjectItem(array_sub, key)) != NULL) {
            break;
        }
    }
    return result;
}


int get_object_array_size(cJSON *array)
{
    return cJSON_GetArraySize(array);
}

cJSON *get_object_with_index_in_array(cJSON *array, int index)
{
    return cJSON_GetArrayItem(array, index);
}