#include <time.h>
#include <stdbool.h>
#include "esp_log.h"
#include "station.h"
#include "led_controller.h"
#include "led.h"
#include "esp_sntp.h"

uint8_t leds[NUM_STATIONS*2];
uint8_t prev_leds[NUM_STATIONS*2];
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

void _check_current_departure(int station_index) {
    station_t station = stations[station_index];

    uint8_t old_led_state_1 = leds[station_index];
    uint8_t old_led_state_2 = leds[49-station_index];

    if (leds[station_index] != LED_CLIGNOTEMENT) {
        leds[station_index] = LED_OFF;
    } 
    if (leds[49-station_index] != LED_CLIGNOTEMENT) {
        leds[49-station_index] = LED_OFF;
    }

    
    for (int i=0; i<MAX_DEPARTURES; i++) {
        if (is_in_range(station.departures1[i], 0, RANGE_SEC_CLIGNOTEMENT)) {
            leds[station_index] = LED_ON;
            break;
        } else if (!is_in_range(station.departures1[i], RANGE_SEC_CLIGNOTEMENT, 5*60) && leds[station_index] == LED_CLIGNOTEMENT) {
            leds[station_index] = LED_OFF;
        }
        /*
        if (is_in_range(station.departures1[i], RANGE_SEC_CLIGNOTEMENT, RANGE_SEC_ON)) {
            leds[station_index] = LED_CLIGNOTEMENT;
            ESP_LOGI(TAG, "Tram en route to %s dest %s", station.name, stations[TERMINUS_1].name);
        } else if (is_in_range(station.departures1[i], 0, RANGE_SEC_CLIGNOTEMENT)) {
            ESP_LOGI(TAG, "Tram @ %s dest %s", station.name, stations[TERMINUS_1].name);
            leds[station_index] = LED_ON;
        }*/
    }
    for (int i=0; i<MAX_DEPARTURES; i++) {
        if (is_in_range(station.departures2[i], 0, RANGE_SEC_CLIGNOTEMENT)) {
            leds[49 - station_index] = LED_ON;
            break;
        } else if (!is_in_range(station.departures2[i], RANGE_SEC_CLIGNOTEMENT, 5*60) && leds[49-station_index] == LED_CLIGNOTEMENT) {
            leds[49-station_index] = LED_OFF;
        }
        /*
        
        if (is_in_range(station.departures2[i], RANGE_SEC_CLIGNOTEMENT, RANGE_SEC_ON)) {
            leds[49-station_index] = LED_CLIGNOTEMENT;
            ESP_LOGI(TAG, "Tram en route to %s dest %s", station.name, stations[TERMINUS_2].name);
        } else if (is_in_range(station.departures2[i], 0, RANGE_SEC_CLIGNOTEMENT)) {
            ESP_LOGI(TAG, "Tram @ %s dest %s", station.name, stations[TERMINUS_2].name);
            leds[49-station_index] = LED_ON;
        }*/
    }

    if ((prev_leds[station_index] == LED_ON) && (leds[station_index] == LED_OFF)) {
        leds[station_index] = LED_CLIGNOTEMENT;
    }
    if ((prev_leds[49-station_index] == LED_ON) && (leds[49-station_index] == LED_OFF)) {
        leds[49-station_index] = LED_CLIGNOTEMENT;
    }

    for(int i=0; i<NUM_STATIONS*2; i++) {
        prev_leds[i] = leds[i];
    }
}

void check_current_departure(int station_index) {
    /*
    is_on: bool
    is_within_arrival (5min): bool

    */
    station_t station = stations[station_index];
    int station_index_2 = 49 - station_index;
    int previous_state_1 = leds[station_index];

    bool is_on = false;
    bool is_within_arrival = false;

    for(int i=0; i<MAX_DEPARTURES; i++) {
        // le tram est a moins de (RANGE_SEC)s du depart: -> led on
        if (is_in_range(station.departures1[i], 0, RANGE_SEC_CLIGNOTEMENT) == true) {
            is_on = true;
        } 
        if (is_in_range(station.departures1[i], RANGE_SEC_CLIGNOTEMENT, 2*60) == true) {
            is_within_arrival = true;
        }
    }
    
    if (is_on) {
        leds[station_index] = LED_ON;
    } else if (is_within_arrival && previous_state_1 == LED_CLIGNOTEMENT) {
        leds[station_index] = LED_CLIGNOTEMENT;
    } else {
        leds[station_index] = LED_OFF;
    }


    if (previous_state_1 == LED_ON && leds[station_index] == LED_OFF) {
        leds[station_index + 1] = LED_CLIGNOTEMENT;
    } else if (previous_state_1 == LED_CLIGNOTEMENT && leds[station_index] == LED_OFF) {
        leds[station_index] = LED_CLIGNOTEMENT;
    } else if (previous_state_1 == LED_OFF && leds[station_index] == LED_CLIGNOTEMENT) {
        leds[station_index] = LED_CLIGNOTEMENT;
    } else if (previous_state_1 == LED_OFF && leds[station_index] == LED_ON) {
        leds[station_index] = LED_ON;
    }

    // debug
    if (leds[station_index] == LED_CLIGNOTEMENT) {
        ESP_LOGI(TAG, "Sta %s range 2min: %d", station.name, is_in_range(station.departures1[0], RANGE_SEC_CLIGNOTEMENT, 2*60));
    }
}

void set_leds(void) {
    for (int i=0; i<NUM_STATIONS*2; i++) {
        if (leds[i] == LED_ON) {
            setPixel(i, 0, 0, BLEU);
        } else if (leds[i] == LED_CLIGNOTEMENT) {
            if (clignotement) {
                setPixel(i, 0, 0, BLEU);
            } else {
                setPixel(i, 0, 0, 0);
            }
        } else {
            setPixel(i, 0, 0, 0);
        }
    }
    clignotement = !clignotement;
    show();
}
