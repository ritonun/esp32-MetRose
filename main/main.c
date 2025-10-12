//main.c
#include "nvs_flash.h"
#include "esp_log.h"
#include "wifi.h"
#include "tisseo.h"
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

    http_wih_url(URL); // from tisseo.c
}