#pragma once

#include "esp_err.h"
#include "cJSON.h"

esp_err_t tisseo_init(const char *API_KEY);
esp_err_t tisseo_get_stop_schedule(const char *stop_id, cJSON **out_json);
void tisseo_free_json(cJSON *json);
void http_wih_url(const char *url);