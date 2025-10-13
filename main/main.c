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
    } else {
        ESP_LOGE("TISSEO", "Failed to get stops schedules");
    }
    
    /*
    const char *fake_response = "{\
        \"departures\": {\
            \"departure\": [{\
                \"dateTime\": \"2025-10-13 17:16:43\",\
                \"destination\": [{\"cityName\": \"BLAGNAC\", \"id\": \"stop_area:SA_1370\", \"name\": \"Odyssud-Ritouret\"}],\
                \"line\": {\"bgXmlColor\": \"#004687\", \"color\": \"(0,70,135)\", \"fgXmlColor\": \"#FFFFFF\", \"id\": \"line:68\", \"name\": \"Palais de Justice / MEETT\", \"network\": \"Tisséo\", \"shortName\": \"T1\"},\
                \"realTime\": \"yes\"\
            }, {\
                \"dateTime\": \"2025-10-13 17:17:33\",\
                \"destination\": [{\"cityName\": \"TOULOUSE\", \"id\": \"stop_area:SA_774\", \"name\": \"Palais de Justice\"}],\
                \"line\": {\"bgXmlColor\": \"#004687\", \"color\": \"(0,70,135)\", \"fgXmlColor\": \"#FFFFFF\", \"id\": \"line:68\", \"name\": \"Palais de Justice / MEETT\", \"network\": \"Tisséo\", \"shortName\": \"T1\"},\
                \"realTime\": \"yes\"\
            }, {\
                \"dateTime\": \"2025-10-13 17:18:23\",\
                \"destination\": [{\"cityName\": \"TOULOUSE\", \"id\": \"stop_area:SA_774\", \"name\": \"Palais de Justice\"}],\
                \"line\": {\"bgXmlColor\": \"#004687\", \"color\": \"(0,70,135)\", \"fgXmlColor\": \"#FFFFFF\", \"id\": \"line:68\", \"name\": \"Palais de Justice / MEETT\", \"network\": \"Tisséo\", \"shortName\": \"T1\"},\
                \"realTime\": \"yes\"\
            }]\
        },\
        \"expirationDate\": \"2025-10-13 17:15\"\
    }\0";

    // Parse JSON
    cJSON *resp = cJSON_Parse(fake_response);
    if (!resp) {
        ESP_LOGE("JSON", "Failed to parse JSON\n");
        return;
    }\0*/

    datetimes_list_str_t *datetimes = extract_departure_time_str(resp);
    if (!datetimes) {
        ESP_LOGE("TISSEO_PARSER", "No departures found");
        return;
    }

    time_t *timestamps = convert_datetimes_str_to_timestamps(datetimes);
    if (!timestamps) {
        ESP_LOGE("TISSEO_PARSER", "Failed to convert departures into timestamps");
        return;
    }

    for (int i = 0; i < datetimes->len; i++) {
        printf("Departure: %s, tm: %lld\n", datetimes->departures_times[i], timestamps[i]);
    }

    while(1) {}
}