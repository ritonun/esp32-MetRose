//station.h
#pragma once

#include <time.h>

#define MAX_DEPARTURES 10
#define MAX_NAME_LEN 32
#define NUM_STATIONS 25
#define STOP_AREA_ID_LEN 20
#define LINE "line:68"

#define TERMINUS_1 (NUM_STATIONS - 1)
#define TERMINUS_2 0

typedef struct {
    int index;                              // LED Index
    char name[MAX_NAME_LEN];                // Station name
    char stop_area_id[STOP_AREA_ID_LEN];    // stop aread id of the station
    time_t departures1[MAX_DEPARTURES];     // timestamps for departures to terminus 1
    time_t departures2[MAX_DEPARTURES];     // timestamps for departures to terminus 2
    int departures_stored;                  // number of timestamps/next departures stored
} station_t;

extern station_t stations[NUM_STATIONS];    // array of station, extern used in other files

// station fn
void init_stations(void);   // init stations_t stations list
void reset_station(int station_index);  // reset all departures to 0
void update_station_departure(int station_index);   // update station departures
