#include <time.h>
#include <stdbool.h>
#include "esp_log.h"
#include "station.h"
#include "led_controller.h"
#include "led.h"
#include "esp_sntp.h"

uint8_t leds[NUM_STATIONS*2];
static const char* TAG = "led";
static bool clignotement = false;

void initialize_sntp(void) {
    ESP_LOGI("TIME", "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}

void init_leds(void) {
    for (int i=0; i<NUM_STATIONS*2; i++) {
        leds[i] = LED_OFF;
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

    leds[station_index] = LED_OFF;
    leds[49 - station_index] = LED_OFF;
    
    for (int i=0; i<MAX_DEPARTURES; i++) {
        if (is_in_range(station.departures1[i], RANGE_SEC_CLIGNOTEMENT, RANGE_SEC_ON)) {
            leds[station_index] = LED_CLIGNOTEMENT;
            ESP_LOGI(TAG, "Tram en route to %s dest %s", station.name, stations[TERMINUS_1].name);
        } else if (is_in_range(station.departures1[i], 0, RANGE_SEC_CLIGNOTEMENT)) {
            ESP_LOGI(TAG, "Tram @ %s dest %s", station.name, stations[TERMINUS_1].name);
            leds[station_index] = LED_ON;
        }
    }
    for (int i=0; i<MAX_DEPARTURES; i++) {
        if (is_in_range(station.departures2[i], RANGE_SEC_CLIGNOTEMENT, RANGE_SEC_ON)) {
            leds[49-station_index] = LED_CLIGNOTEMENT;
            ESP_LOGI(TAG, "Tram en route to %s dest %s", station.name, stations[TERMINUS_2].name);
        } else if (is_in_range(station.departures2[i], 0, RANGE_SEC_CLIGNOTEMENT)) {
            ESP_LOGI(TAG, "Tram @ %s dest %s", station.name, stations[TERMINUS_2].name);
            leds[49-station_index] = LED_ON;
        }
    }
    //set_leds();
}

void set_leds(void) {
    ESP_LOGE(TAG, "SET LEDS");
    for (int i=0; i<NUM_STATIONS*2; i++) {
        if (leds[i] == LED_CLIGNOTEMENT) {
            if (clignotement) {
                setPixel(i, 0, 0, 255);
            } else {
                setPixel(i, 0, 0, 0);
            }
        } else if (leds[i] == LED_ON) {
            setPixel(i, 0, 0, 255);
        } else {
            setPixel(i, 0, 0, 0);
        }
    }
    clignotement = !clignotement;
    show();
}

void clignotement_leds(void) {
    ESP_LOGD(TAG, "Clignotement des leds");
    static bool clignotement = false;

    for (int i=0; i<NUM_STATIONS*2; i++) {
        if (leds[i] == LED_CLIGNOTEMENT) {
            if (clignotement) {
                setPixel(i, 0, 0, 255);
            } else {
                setPixel(i, 0, 0, 0);
            }
        }
    }
    clignotement = !clignotement;
}