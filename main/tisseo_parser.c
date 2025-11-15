//tisseo_parser.c
#include "esp_log.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tisseo_parser.h"
#include "station.h"

const char *TAG = "tisseo_parser";

datetimes_list_str_t *extract_departure_time_str(cJSON *resp, int station_index) {
    // get json object
    cJSON *departures = cJSON_GetObjectItem(resp, "departures");

    // extract departures["departure"] []
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

    datetimes->departures_times_1 = malloc(sizeof(char*) * size);
    if (!datetimes->departures_times_1) {
        ESP_LOGE(TAG, "Failed to allocate datetimes array n1");
        free(datetimes);
        cJSON_Delete(resp);
        return NULL;
    }
    datetimes->departures_times_2 = malloc(sizeof(char*) * size);
    if (!datetimes->departures_times_2) {
        ESP_LOGE(TAG, "Failed to allocate datetimes array n2");
        free(datetimes);
        cJSON_Delete(resp);
        return NULL;
    }

    int i1 = 0;
    int i2 = 0;
    for (int i=0; i<size; i++) {
        cJSON *item = cJSON_GetArrayItem(departures_array, i);
        cJSON *datetime;
        if (item) {
            datetime = cJSON_GetObjectItem(item, "dateTime");
        } else {
            datetime = NULL;
        }

        // get terminus
        cJSON *destination;
        if (item) {
            destination = cJSON_GetObjectItem(item, "destination");
        } else {
            destination = NULL;
        }
        // validate
        if (!destination || !cJSON_IsArray(destination)) {
            ESP_LOGE(TAG, "No destination array found\n");
            return NULL;
        }
        // Get the first element of the array
        cJSON *first_dest = cJSON_GetArrayItem(destination, 0);
        if (!first_dest || !cJSON_IsObject(first_dest)) {
            ESP_LOGE(TAG, "First destination is not an object\n");
            return NULL;
        }

        // Get the "id" field
        cJSON *id_item = cJSON_GetObjectItem(first_dest, "id");
        if (!id_item || !cJSON_IsString(id_item)) {
            ESP_LOGE(TAG, "No id found in destination\n");
            return NULL;
        }
        const char *terminus_stop_area_id = id_item->valuestring;

        if (datetime && cJSON_IsString(datetime)) {
            if (strcmp(terminus_stop_area_id, stations[TERMINUS_1].stop_area_id) == 0) {
                ESP_LOGI(TAG, "matched station: %s, T1: %s,", terminus_stop_area_id, stations[TERMINUS_1].stop_area_id);
                datetimes->departures_times_1[i1++] = strdup(datetime->valuestring); // copy str
            }
            else if (strcmp(terminus_stop_area_id, stations[TERMINUS_2].stop_area_id) == 0) {
                ESP_LOGI(TAG, "Matched station: %s, T2: %s,", terminus_stop_area_id, stations[TERMINUS_2].stop_area_id);
                datetimes->departures_times_2[i2++] = strdup(datetime->valuestring); // copy str
            }
            else {
                ESP_LOGE(TAG, "Failed to match terminus (774 or 2334) to station %s", terminus_stop_area_id);
            }
        } else {
            ESP_LOGW(TAG, "Invalid datetime string at index %d", i);
        }
    }

    datetimes->len_1 = i1;
    datetimes->len_2 = i2;
    //for (int i = i1; i < size; i++) datetimes->departures_times_1[i] = NULL;
    //for (int i = i2; i < size; i++) datetimes->departures_times_2[i] = NULL;


    cJSON_Delete(resp);

    return datetimes;
}

time_t* convert_datetimes_str_to_timestamps(datetimes_list_str_t *datetimes, int terminus) {
    if (terminus == 1) {
        if (!datetimes || datetimes->len_1 == 0) {
            ESP_LOGE(TAG, "List of datetimes_str is empty/does not exist");
            return NULL;
        }

        time_t *timestamps = malloc(sizeof(time_t) * datetimes->len_1);
        if (!timestamps) {
            ESP_LOGE(TAG, "Failed to create timestamp memory space");
            return NULL;
        }

        for (int i=0; i<datetimes->len_1; i++) {
            struct tm tm_time = {0};
            if (strptime(datetimes->departures_times_1[i], "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
                timestamps[i] = mktime(&tm_time);
            } else {
                timestamps[i] = -1000;
            }
        }

        return timestamps;
    } else if (terminus == 2) {
        if (!datetimes || datetimes->len_2 == 0) {
            ESP_LOGE(TAG, "List of datetimes_str is empty/does not exist");
            return NULL;
        }

        time_t *timestamps = malloc(sizeof(time_t) * datetimes->len_2);
        if (!timestamps) {
            ESP_LOGE(TAG, "Failed to create timestamp memory space");
            return NULL;
        }

        for (int i=0; i<datetimes->len_2; i++) {
            struct tm tm_time = {0};
            if (strptime(datetimes->departures_times_2[i], "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
                timestamps[i] = mktime(&tm_time);
            } else {
                timestamps[i] = -1000;
            }
        }

        return timestamps;
    } else {
        ESP_LOGE(TAG, "Terminus value must be 1 or 2. It is %d", terminus);
        return NULL;
    }
}