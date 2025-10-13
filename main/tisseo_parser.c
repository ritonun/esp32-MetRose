//tisseo_parser.c
#include "esp_log.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tisseo_parser.h"

const char *TAG = "tisseo_parser";

datetimes_list_str_t *extract_departure_time_str(cJSON *resp)
{
    cJSON *departures = cJSON_GetObjectItem(resp, "departures");

    cJSON *departures_array;
    if (departures) {
        departures_array = cJSON_GetObjectItem(departures, "departure");
    } else {
        departures_array = NULL;
    }
    
    if (!departures_array || !cJSON_IsArray(departures_array)) {
        ESP_LOGE(TAG, "No departure found / failed to properly pars the json");
        cJSON_Delete(resp);
        return NULL;
    }

    int size = cJSON_GetArraySize(departures_array);
    if (size == 0) {
        ESP_LOGI(TAG, "No departure found.");
        cJSON_Delete(resp);
        return NULL;
    }

    // allocate memory
    datetimes_list_str_t *datetimes = malloc(sizeof(datetimes_list_str_t));
    if (!datetimes) {
        ESP_LOGE(TAG, "Failed to allocate datetime list memory");
        cJSON_Delete(resp);
        return NULL;
    }

    datetimes->departures_times = malloc(sizeof(char*) * size);
    if (!datetimes->departures_times) {
        ESP_LOGE(TAG, "Failed to allocate datetimes array");
        free(datetimes);
        cJSON_Delete(resp);
        return NULL;
    }
    datetimes->len = size;

    for (int i=0; i<size; i++) {
        cJSON *item = cJSON_GetArrayItem(departures_array, i);
        cJSON *datetime;
        if (item) {
            datetime = cJSON_GetObjectItem(item, "dateTime");
        } else {
            datetime = NULL;
        }

        if (datetime && cJSON_IsString(datetime)) {
            datetimes->departures_times[i] = strdup(datetime->valuestring); // copy str
        } else {
            datetimes->departures_times = NULL;
        }
    }

    cJSON_Delete(resp);

    return datetimes;
}

time_t* convert_datetimes_str_to_timestamps(datetimes_list_str_t *datetimes) {
    if (!datetimes || datetimes->len == 0) {
        ESP_LOGE(TAG, "List of datetimes_str is empty/does not exist");
        return NULL;
    }

    time_t *timestamps = malloc(sizeof(time_t) * datetimes->len);
    if (!timestamps) {
        ESP_LOGE(TAG, "Failed to create timestamp memory space");
        return NULL;
    }

    for (int i=0; i<datetimes->len; i++) {
        struct tm tm_time = {0};
        if (strptime(datetimes->departures_times[i], "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
            timestamps[i] = mktime(&tm_time);
        } else {
            timestamps[i] = -1;
        }
    }

    return timestamps;
}