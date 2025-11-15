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
#include "led.h"

#include "led_controller.h"

#define DELAY_API_STATION_UPDATE 5*60*1000 // 5 min
#define DELAY_LED_UPDATE 2*1000 // 1
#define DELAY_BLINK_LED 2*250 // 500ms
#define LED_TASK_PRIORITY 5
#define API_TASK_PRIORITY 2
#define BLINK_LED_PRIORITY 8


struct tm timeinfo = { 0 };
static const char* TAG = "main";

void print_memory(void) {
    size_t free_heap = esp_get_free_heap_size();
    ESP_LOGI("MEMORY", "Free heap: %u bytes", free_heap);
    size_t min_free_heap = esp_get_minimum_free_heap_size();
    ESP_LOGI("MEMORY", "Minimum ever free heap: %u bytes", min_free_heap);
}

void update_station_api_call(void *pvParameters) {
    const TickType_t delay_ticks = pdMS_TO_TICKS(DELAY_API_STATION_UPDATE); // 5 minutes

    while (1) {
        // appel api pour actualiser les departures times de toutes les stations
        for (int s=0; s<NUM_STATIONS; s++) {
            update_station_departure(s);
            print_memory();
            taskYIELD();
        }

        // Wait before running again
        vTaskDelay(delay_ticks);
    }
}

void update_led(void *pvParameters) {
    const TickType_t delay_ticks = pdMS_TO_TICKS(DELAY_LED_UPDATE);

    while (1) {

        // update led
        for (int i=0; i<NUM_STATIONS; i++) {
            check_current_departure(i);
        }

        vTaskDelay(delay_ticks);
    }
}

void blink_led(void *pvParameters) {
    const TickType_t delay_ticks = pdMS_TO_TICKS(DELAY_BLINK_LED);

    while (1) {

        // blink led
        set_leds();

        vTaskDelay(delay_ticks);
    }
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

    ESP_LOGI(TAG, "Démarrage wifi...");
    wifi_init_sta();  // from wifi.c

    led_manager_init(50);

    initialize_sntp();
    
    init_leds();

    time_t now = 0;
    int retry = 0;
    const int retry_count = 10;
    while(timeinfo.tm_year < (2020 - 1900) && ++retry < retry_count) {
        ESP_LOGI("TIME", "Waiting for system time to be set...");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
    // initialisation de la liste de struct contenant les infos stations
    init_stations();

    xTaskCreate(
        update_station_api_call,
        "update_station_api_call",
        4096,
        NULL,
        API_TASK_PRIORITY,
        NULL
    );
    xTaskCreate(
        update_led,
        "update_led",
        4096,
        NULL,
        LED_TASK_PRIORITY,
        NULL
    );
    xTaskCreate(
        blink_led,
        "blink_led",
        2048,
        NULL,
        BLINK_LED_PRIORITY,
        NULL
    );
}