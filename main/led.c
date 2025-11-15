#include <time.h>
#include <stdbool.h>
#include "esp_log.h"
#include "station.h"
#include "led_controller.h"

#include "esp_sntp.h"

void initialize_sntp(void) {
    ESP_LOGI("TIME", "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}

static const char* TAG = "led";

#define RANGE_SEC 45

bool is_in_range(time_t ts) {
    if (ts <= 0) {
        return false;
    }
    time_t now = time(NULL) + 3600;
    int64_t diff = (int64_t)ts - (int64_t)now;

    return (diff >= 0 && diff <= 60);
}

void check_current_departure(int station_index) {
    station_t station = stations[station_index];
    /*
    for (int i=0; i<MAX_DEPARTURES; i++) {
        int64_t diff = is_in_range(station.departures1[i]);

        if (diff >= -30 && diff <= 0) {
            setPixel(49 - station_index, 0, 0, 255);
            ESP_LOGI(TAG, "Tram @ %s to %s", station.name, stations[TERMINUS_1].name);
            break;
        } else if (diff >= -90 && diff < -30) {
            setPixel(49 - station_index, 0, 0, 125);
            ESP_LOGI(TAG, "Tram arrival %s to %s", station.name, stations[TERMINUS_1].name);
            break;
        } else {
            setPixel(49 - station_index, 0, 0, 0);
        }
        show();
    }

    for (int i=0; i<MAX_DEPARTURES; i++) {
        int64_t diff = is_in_range(station.departures2[i]);

        if (diff >= -30 && diff <= 0) {
            setPixel(station_index, 0, 0, 255);
            ESP_LOGI(TAG, "Tram @ %s to %s", station.name, stations[TERMINUS_1].name);
            break;
        } else if (diff >= -90 && diff < -30) {
            setPixel(station_index, 0, 0, 125);
            ESP_LOGI(TAG, "Tram arrival %s to %s", station.name, stations[TERMINUS_1].name);
            break;
        } else {
            setPixel(station_index, 0, 0, 0);
        }
        show();
    }*/

    
    bool is_tram_1 = false;
    bool is_tram_2 = false;
    for (int i=0; i<MAX_DEPARTURES; i++) {
        if (is_in_range(station.departures1[i])) {
            is_tram_1 = true;
            ESP_LOGI(TAG, "Tram @ %s to %s", station.name, stations[TERMINUS_1].name);
        }
        if (is_in_range(station.departures2[i])) {
            is_tram_2 = true;
            ESP_LOGI(TAG, "Tram @ %s to %s", station.name, stations[TERMINUS_2].name);
        }
        if (is_tram_1) {
            setPixel(49-station_index, 0, 0, 255);
        } else {
            setPixel(49-station_index, 0, 0, 0);
        }

        if (is_tram_2) {
            setPixel(station_index, 0, 0, 255);
        } else {
            setPixel(station_index, 0, 0, 0);
        }
        show();
    }
        
}