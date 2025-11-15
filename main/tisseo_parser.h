//tisseo_parser.h
#pragma once

#include "esp_err.h"
#include "cJSON.h"
#include "time.h"

typedef struct {
    char **departures_times_1;
    char **departures_times_2;
    int len_1;
    int len_2;
} datetimes_list_str_t;


datetimes_list_str_t *extract_departure_time_str(cJSON *resp, int station_index);
time_t* convert_datetimes_str_to_timestamps(datetimes_list_str_t *datetimes, int terminus);