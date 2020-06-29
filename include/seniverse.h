#ifndef SENIVERSE_H
#define SENIVERSE_H

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "lwip/apps/sntp.h"
#include "lwip/dns.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include "esp_tls.h"

#include "cJSON.h"
#include "seniverse_weather.h"

int seniverse_get_weather(enum SENIVERSE_WEATHER_DATA_TYPE type, char *location,
                          void *data, int *count, int start, int cnt);



#endif // SENIVERSE_H