//station.c
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>
#include <stdio.h>
#include "station.h"
#include "tisseo.h"
#include "tisseo_parser.h"

static const char* TAG = "station";

station_t stations[NUM_STATIONS];

void init_stations(void) {
    /*Initialise each station*/
    stations[0].index = 0;
    strncpy(stations[0].name, "Hippodrome", MAX_NAME_LEN);
    strncpy(stations[0].stop_area_id, "stop_area:SA_731", STOP_AREA_ID_LEN);
    reset_station(stations[0].index);

    stations[1].index = 1;
    strncpy(stations[1].name, "Zenith", MAX_NAME_LEN);
    strncpy(stations[1].stop_area_id, "stop_area:SA_987", STOP_AREA_ID_LEN);
    reset_station(stations[1].index);
}

void reset_station(int station_index) {
    for (int i=0; i<MAX_DEPARTURES; i++) {
        stations[station_index].departures[i] = 0;
    }
    stations[station_index].departures_stored = 0;
}

void update_station_departure(int station_index) {
    
    // make api request for station
    cJSON *resp = tisseo_get_stops_schedules(stations[station_index].stop_area_id, LINE);
    if (resp) {
        ESP_LOGI(TAG, "Response for station %s OK", stations[station_index].name);
    } else {
        ESP_LOGE(TAG, "Failed to get stops schedules for station %s", stations[station_index].name);
        return;
    }

    // extract datetimes from the json response
    datetimes_list_str_t *datetimes = extract_departure_time_str(resp);
    if (!datetimes || datetimes->len == 0) {
        ESP_LOGE("STATION", "Datetimes: No departures found");
        cJSON_Delete(resp);
        return;
    }

    // convert the datetimes str into timestamps
    time_t *timestamps = convert_datetimes_str_to_timestamps(datetimes);
    if (!timestamps) {
        ESP_LOGE("STATION", "Timestamps: Failed to convert departures into timestamps");
        cJSON_Delete(resp);
        free(datetimes);
        return;
    }



    // reset station data before putting new data
    reset_station(station_index);

    // store timestamp safely
    int store_count;
    if (datetimes->len < MAX_DEPARTURES) {
        store_count = datetimes->len;
    } else {
        store_count = MAX_DEPARTURES;
    }
    for (int i = 0; i < store_count; i++) {
        stations[station_index].departures[i] = timestamps[i];
    }
    stations[station_index].departures_stored = store_count;

    // temporary code, print the datetimes and timestamps
    for (int i = 0; i < datetimes->len; i++) {
        printf("Departure: %s, tm: %lld\n", datetimes->departures_times[i], timestamps[i]);
    }


    // free memory
    free(timestamps);
    free(datetimes);
    cJSON_Delete(resp);
}