//tisseo.h
#pragma once

#include "esp_err.h"
#include "cJSON.h"

cJSON *tisseo_get_stops_schedules(int station_index);
