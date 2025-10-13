//main.c
#include "nvs_flash.h"
#include "esp_log.h"
#include "wifi.h"
#include "tisseo.h"
#include "tisseo_parser.h"
#include "config.h"

static const char* TAG = "main";

void app_main(void)
{
    // Init non-volatile storage (NVS)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Starting Wi-Fi Station example...");
    wifi_init_sta();  // from wifi.c

    // tisseo.c call
    const char *stop_area = "stop_area:SA_731";
    const char *line_id = "line:68";
    cJSON *resp = tisseo_get_stops_schedules(stop_area, line_id);
    if (resp) {
        char *pretty = cJSON_Print(resp);
        ESP_LOGI("TISSEO", "Response:\n%s", pretty);
        free(pretty);
        cJSON_Delete(resp);
    } else {
        ESP_LOGE("TISSEO", "Failed to get stops schedules");
    }

    datetimes_list_str_t *datetimes = extract_departure_time_str(resp);
    if (!datetimes) {
        ESP_LOGE("TISSEO", "No departures found");
        return;
    }

    for (int i = 0; i < datetimes->len; i++) {
        if (datetimes->departures_times[i]) {
            printf("Departure: %s\n", datetimes->departures_times[i]);
    
        }
    }
}