//main.c
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tisseo.h"
#include "tisseo_parser.h"
#include "config.h"
#include "station.h"

static const char* TAG = "main";

void print_memory(void) {
    size_t free_heap = esp_get_free_heap_size();
    ESP_LOGI("MEMORY", "Free heap: %u bytes", free_heap);
    size_t min_free_heap = esp_get_minimum_free_heap_size();
    ESP_LOGI("MEMORY", "Minimum ever free heap: %u bytes", min_free_heap);
}

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
    
    // print_memory();
    init_stations();
    // print_memory();
    for (int s=0; s<NUM_STATIONS; s++) {
        update_station_departure(s);
        print_memory();
    }
    
    vTaskDelay(pdMS_TO_TICKS(5 * 60 * 1000));

}