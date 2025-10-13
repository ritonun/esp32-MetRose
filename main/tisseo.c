//tisseo.c
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

#include "tisseo.h"
#include "config.h"

static const char *TAG = "tisseo";
static const char *BASE_URL = "https://api.tisseo.fr/v2/stops_schedules.json";

typedef struct {
    char *buffer;
    int len;
    int status_code;
    esp_err_t err;
} tisseo_response_t;


static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    tisseo_response_t *resp = (tisseo_response_t *)evt->user_data;

    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA: {
            // Allocate or expand buffer dynamically
            char *tmp = realloc(resp->buffer, resp->len + evt->data_len + 1);
            if (tmp == NULL) {
                ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                free(resp->buffer);
                resp->buffer = NULL;
                resp->err = ESP_ERR_NO_MEM;
                return ESP_FAIL;
            }
            resp->buffer = tmp;
            memcpy(resp->buffer + resp->len, evt->data, evt->data_len);
            resp->len += evt->data_len;
            resp->buffer[resp->len] = '\0'; // Null Terminate
            break;
        }
        case HTTP_EVENT_ON_FINISH:
            break;
        case HTTP_EVENT_DISCONNECTED:
            break;
        default:
            break;
    }
    return ESP_OK;
}

static tisseo_response_t http_get(const char *url)
{
    tisseo_response_t resp = {
        .buffer = NULL,
        .status_code = 0,
        .len = 0,
        .err = ESP_OK,
    };

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = http_event_handler,
        .user_data = &resp,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG, "Failed to init http client");
        resp.err = ESP_FAIL;
        return resp;
    }

    resp.err = esp_http_client_perform(client);

    if (resp.err == ESP_OK) {
        resp.status_code = esp_http_client_get_status_code(client);
        resp.len = esp_http_client_get_content_length(client);
        ESP_LOGI(TAG, "HTTP %d, %d bytes", resp.status_code, resp.len);
    } else {
        ESP_LOGE(TAG, "HTTP perform failed: %s", esp_err_to_name(resp.err));
    }

    esp_http_client_cleanup(client);
    return resp;
}

cJSON *tisseo_get_stops_schedules(const char *stop_area_id, const char *line_id)
{
    char url[256];
    int write_url = snprintf(url, sizeof(url),
                    "%s?stopAreaId=%s&lineId=%s&key=%s",
                    BASE_URL, stop_area_id, line_id, API_KEY);
    if (write_url < 0 || write_url >= sizeof(url)) {
        ESP_LOGE(TAG, "Query parameter to be written in the url overflow 256 char");
        return NULL;
    }

    ESP_LOGI(TAG, "URL:%s", url);

    tisseo_response_t resp = http_get(url);
    if (resp.err != ESP_OK || resp.status_code != 200 || !resp.buffer) {
        ESP_LOGE(TAG, "HTTP request failed");
        free(resp.buffer);
        return NULL;
    }


    cJSON *json = cJSON_Parse(resp.buffer);
    if (!json) {
        ESP_LOGE(TAG, "Failed to parse JSON");
    }

    free(resp.buffer); // free the raw response buffer
    return json;       // caller must free with cJSON_Delete()
}