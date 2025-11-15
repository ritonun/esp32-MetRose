#pragma once

#include <time.h>

bool is_in_range(time_t ts);
void check_current_departure(int station_index);
void initialize_sntp(void);