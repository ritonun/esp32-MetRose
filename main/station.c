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
    strncpy(stations[0].name, "Palais de Justice", MAX_NAME_LEN);
    strncpy(stations[0].stop_area_id, "stop_area:SA_774", STOP_AREA_ID_LEN);
    reset_station(stations[0].index);

    stations[1].index = 1;
    strncpy(stations[1].name, "Ile du Ramier", MAX_NAME_LEN);
    strncpy(stations[1].stop_area_id, "stop_area:SA_998", STOP_AREA_ID_LEN);
    reset_station(stations[1].index);

    stations[2].index = 2;
    strncpy(stations[2].name, "Fer à Cheval", MAX_NAME_LEN);
    strncpy(stations[2].stop_area_id, "stop_area:SA_189", STOP_AREA_ID_LEN);
    reset_station(stations[2].index);

    stations[3].index = 3;
    strncpy(stations[3].name, "Avenue Muret Marcel Cavaillé", MAX_NAME_LEN);
    strncpy(stations[3].stop_area_id, "stop_area:SA_667", STOP_AREA_ID_LEN);
    reset_station(stations[3].index);

    stations[4].index = 4;
    strncpy(stations[4].name, "Croix de Pierre", MAX_NAME_LEN);
    strncpy(stations[4].stop_area_id, "stop_area:SA_1078", STOP_AREA_ID_LEN);
    reset_station(stations[4].index);

    stations[5].index = 5;
    strncpy(stations[5].name, "Déodat de Séverac", MAX_NAME_LEN);
    strncpy(stations[5].stop_area_id, "stop_area:SA_1311", STOP_AREA_ID_LEN);
    reset_station(stations[5].index);

    stations[6].index = 6;
    strncpy(stations[6].name, "Arènes", MAX_NAME_LEN);
    strncpy(stations[6].stop_area_id, "stop_area:SA_364", STOP_AREA_ID_LEN);
    reset_station(stations[6].index);

    stations[7].index = 7;
    strncpy(stations[7].name, "Hippodrome", MAX_NAME_LEN);
    strncpy(stations[7].stop_area_id, "stop_area:SA_731", STOP_AREA_ID_LEN);
    reset_station(stations[7].index);

    stations[8].index = 8;
    strncpy(stations[8].name, "Zénith", MAX_NAME_LEN);
    strncpy(stations[8].stop_area_id, "stop_area:SA_987", STOP_AREA_ID_LEN);
    reset_station(stations[8].index);

    stations[9].index = 9;
    strncpy(stations[9].name, "Cartoucherie", MAX_NAME_LEN);
    strncpy(stations[9].stop_area_id, "stop_area:SA_653", STOP_AREA_ID_LEN);
    reset_station(stations[9].index);

    stations[10].index = 10;
    strncpy(stations[10].name, "Purpan", MAX_NAME_LEN);
    strncpy(stations[10].stop_area_id, "stop_area:SA_1863", STOP_AREA_ID_LEN);
    reset_station(stations[10].index);

    stations[11].index = 11;
    strncpy(stations[11].name, "Arènes Romaines", MAX_NAME_LEN);
    strncpy(stations[11].stop_area_id, "stop_area:SA_1742", STOP_AREA_ID_LEN);
    reset_station(stations[11].index);

    stations[12].index = 12;
    strncpy(stations[12].name, "Ancely", MAX_NAME_LEN);
    strncpy(stations[12].stop_area_id, "stop_area:SA_95", STOP_AREA_ID_LEN);
    reset_station(stations[12].index);

    stations[13].index = 13;
    strncpy(stations[13].name, "Servanty Airbus", MAX_NAME_LEN);
    strncpy(stations[13].stop_area_id, "stop_area:SA_730", STOP_AREA_ID_LEN);
    reset_station(stations[13].index);

    stations[14].index = 14;
    strncpy(stations[14].name, "Guyenne-Berry", MAX_NAME_LEN);
    strncpy(stations[14].stop_area_id, "stop_area:SA_1531", STOP_AREA_ID_LEN);
    reset_station(stations[14].index);

    stations[15].index = 15;
    strncpy(stations[15].name, "Pasteur-Mairie de Blagnac", MAX_NAME_LEN);
    strncpy(stations[15].stop_area_id, "stop_area:SA_1229", STOP_AREA_ID_LEN);
    reset_station(stations[15].index);

    stations[16].index = 16;
    strncpy(stations[16].name, "Place du Relais", MAX_NAME_LEN);
    strncpy(stations[16].stop_area_id, "stop_area:SA_447", STOP_AREA_ID_LEN);
    reset_station(stations[16].index);

    stations[17].index = 17;
    strncpy(stations[17].name, "Odyssud-Ritouret", MAX_NAME_LEN);
    strncpy(stations[17].stop_area_id, "stop_area:SA_1370", STOP_AREA_ID_LEN);
    reset_station(stations[17].index);

    stations[18].index = 18;
    strncpy(stations[18].name, "Patinoire-Barradels", MAX_NAME_LEN);
    strncpy(stations[18].stop_area_id, "stop_area:SA_1418", STOP_AREA_ID_LEN);
    reset_station(stations[18].index);

    stations[19].index = 19;
    strncpy(stations[19].name, "Grand Noble", MAX_NAME_LEN);
    strncpy(stations[19].stop_area_id, "stop_area:SA_573", STOP_AREA_ID_LEN);
    reset_station(stations[19].index);

    stations[20].index = 20;
    strncpy(stations[20].name, "Place Georges Brassens", MAX_NAME_LEN);
    strncpy(stations[20].stop_area_id, "stop_area:SA_138", STOP_AREA_ID_LEN);
    reset_station(stations[20].index);

    stations[21].index = 21;
    strncpy(stations[21].name, "Andromède-Lycée", MAX_NAME_LEN);
    strncpy(stations[21].stop_area_id, "stop_area:SA_636", STOP_AREA_ID_LEN);
    reset_station(stations[21].index);

    stations[22].index = 22;
    strncpy(stations[22].name, "Beauzelle  - Aeroscopia", MAX_NAME_LEN);
    strncpy(stations[22].stop_area_id, "stop_area:SA_2525", STOP_AREA_ID_LEN);
    reset_station(stations[22].index);

    stations[23].index = 23;
    strncpy(stations[23].name, "Aéroconstellation", MAX_NAME_LEN);
    strncpy(stations[23].stop_area_id, "stop_area:SA_624", STOP_AREA_ID_LEN);
    reset_station(stations[23].index);

    stations[24].index = 24;
    strncpy(stations[24].name, "MEETT", MAX_NAME_LEN);
    strncpy(stations[24].stop_area_id, "stop_area:SA_2334", STOP_AREA_ID_LEN);
    reset_station(stations[24].index);

}

void reset_station(int station_index) {
    for (int i=0; i<MAX_DEPARTURES; i++) {
        stations[station_index].departures[i] = 0;
    }
    stations[station_index].departures_stored = 0;
}

void update_station_departure(int station_index) {
    
    // make api request for station
    ESP_LOGI(TAG, "Trying station %s", stations[station_index].name);

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
    ESP_LOGI("FREE", "TIMESTAMP:");
    free(timestamps);
    ESP_LOGI("FREE", "DATETIMES:");
    free(datetimes);
}