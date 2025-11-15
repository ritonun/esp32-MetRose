#pragma once

#include <time.h>
#include "station.h"

#define LED_OFF 100
#define LED_CLIGNOTEMENT 101
#define LED_ON 102
#define RANGE_SEC_CLIGNOTEMENT 30
#define RANGE_SEC_ON 70

extern uint8_t leds[NUM_STATIONS*2];

bool is_in_range(time_t ts, int min_range, int max_range);
void check_current_departure(int station_index);
void initialize_sntp(void);
void init_leds(void);
void set_leds(void);