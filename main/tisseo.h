//tisseo.h
#pragma once

#include "esp_err.h"
#include "cJSON.h"

esp_err_t tisseo_init(const char *API_KEY);
void *tisseo_request(const char *service, const char *format, const char *params_query);
cJSON *tisseo_get_stops_schedules(const char *stop_area_id, const char *line_id);
void tisseo_deinit(void);
