//tisseo.c
#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

#include "tisseo.h"

static const char *TAG = "tisseo";

static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer = NULL;
    static int output_len = 0;

    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA: {
            // Allocate or expand buffer dynamically
            char *tmp = realloc(output_buffer, output_len + evt->data_len + 1);
            if (tmp == NULL) {
                ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                free(output_buffer);
                output_buffer = NULL;
                output_len = 0;
                return ESP_FAIL;
            }
            output_buffer = tmp;
            memcpy(output_buffer + output_len, evt->data, evt->data_len);
            output_len += evt->data_len;
            output_buffer[output_len] = 0;  // null terminate
            break;
        }
        case HTTP_EVENT_ON_FINISH:
            if (output_buffer != NULL) {
                ESP_LOGI(TAG, "Response:\n%s", output_buffer);
                free(output_buffer);
                output_buffer = NULL;
                output_len = 0;
            }
            break;
        case HTTP_EVENT_DISCONNECTED:
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
                output_len = 0;
            }
            break;
        default:
            break;
    }
    return ESP_OK;
}

void http_wih_url(const char *url)
{
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);
}