#include <time.h>
#include <stdbool.h>
#include "esp_log.h"
#include "station.h"
#include "led_controller.h"
#include "led.h"
#include "esp_sntp.h"

uint8_t leds[NUM_STATIONS*2];
static const char* TAG = "led";

void initialize_sntp(void) {
    ESP_LOGI("TIME", "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}

void init_leds(void) {
    for (int i=0; i<NUM_STATIONS*2; i++) {
        leds[i] = 0;
    }
}


bool is_in_range(time_t ts, int min_range, int max_range) {
    if (ts <= 0) {
        return false;
    }
    time_t now = time(NULL) + 3600;
    int64_t diff = (int64_t)ts - (int64_t)now;

    return (diff >= (int64_t)min_range && diff <= (int64_t)max_range);
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

    
    for (int i=0; i<MAX_DEPARTURES; i++) {
        if (is_in_range(station.departures1[i], RANGE_SEC_CLIGNOTEMENT, RANGE_SEC_ON)) {
            leds[i] = LED_CLIGNOTEMENT;
            ESP_LOGI(TAG, "Tram en route to %s dest %s", station.name, stations[TERMINUS_1].name);
        } else if (is_in_range(station.departures1[i], 0, RANGE_SEC_CLIGNOTEMENT)) {
            ESP_LOGI(TAG, "Tram @ %s dest %s", station.name, stations[TERMINUS_1].name);
            leds[i] = LED_ON;
        } else {
            leds[i] = LED_OFF;
        }

        if (is_in_range(station.departures2[i], RANGE_SEC_CLIGNOTEMENT, RANGE_SEC_ON)) {
            leds[49-i] = LED_CLIGNOTEMENT;
            ESP_LOGI(TAG, "Tram en route to %s dest %s", station.name, stations[TERMINUS_2].name);
        } else if (is_in_range(station.departures2[i], 0, RANGE_SEC_CLIGNOTEMENT)) {
            ESP_LOGI(TAG, "Tram @ %s dest %s", station.name, stations[TERMINUS_2].name);
            leds[49-i] = LED_ON;
        } else {
            leds[49-i] = LED_OFF;
        }
    }
    set_leds();
}

void set_leds(void) {
    ESP_LOGE(TAG, "SET LEDS");
    clearAll();
    for (int i=0; i<NUM_STATIONS*2; i++) {
        if (leds[i] == LED_CLIGNOTEMENT) {
            setPixel(i, 200, 0, 0);
        } else if (leds[i] == LED_ON) {
            setPixel(i, 0, 0, 255);
        }
    }
    show();
}