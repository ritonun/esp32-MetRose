//tisseo.h
#pragma once

#include "esp_err.h"
#include "cJSON.h"

cJSON *tisseo_get_stops_schedules(const char *stop_area_id, const char *line_id);

