#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "soc/rtc_cntl_reg.h"

#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"

#include "user_wifi.h"
static const char *TAG = "App";


void app_main()
{
  esp_chip_info_t chip_info;
  esp_chip_info( &chip_info );

  ESP_LOGI(TAG, "Opening file");

	ESP_LOGI(TAG, "\r\n\r\n-------------------------------------------------\r\n\r\n");
  ESP_LOGI(TAG, "FLASH SIZE: %s  %d MB  \r\n", (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external",
												spi_flash_get_chip_size() / (1024 * 1024));
  ESP_LOGI(TAG, "Free memory: %d bytes \r\n", esp_get_free_heap_size());

  nvs_flash_init();
  user_wifi_config();
}
