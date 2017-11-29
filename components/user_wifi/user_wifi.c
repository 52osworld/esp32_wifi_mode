#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "user_wifi.h"

#if CONFIG_WIFI_MODE_TYPE_STA
static const char *TAG = "WIFI_STA";
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	wifi_ap_record_t my_wifi_info;

  switch(event->event_id) {
		case SYSTEM_EVENT_STA_START:
			ESP_LOGI(TAG, "STA start event!\r\n");
			ESP_ERROR_CHECK(esp_wifi_connect());
			break;
		case SYSTEM_EVENT_STA_GOT_IP:
			ESP_LOGI(TAG, "Get IP event!\r\n");
			ESP_LOGI(TAG, "ESP32 IP: %s !\r\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
			break;
		case SYSTEM_EVENT_STA_CONNECTED:
			ESP_LOGI(TAG, "Wifi STA connect event!\r\n");
			esp_wifi_sta_get_ap_info(&my_wifi_info);
			ESP_LOGI(TAG, "Connect to : %s!\r\n", my_wifi_info.ssid);
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:
			ESP_LOGI(TAG, "Wifi STA disconnect event, reconnect!\r\n");
			ESP_ERROR_CHECK( esp_wifi_connect() );
			break;
		default:
			break;
  }

  return ESP_OK;
}


void user_wifi_config(void)
{
	ESP_LOGI(TAG, "STA config start, connect to Wifi : %s \r\n", CONFIG_STA_SSID);

	tcpip_adapter_init();

	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );

  wifi_config_t wifi_sta_config = {
		.sta = {
			.ssid = CONFIG_STA_SSID,
			.password = CONFIG_STA_PASSWORD
		},
  };
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
  ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_sta_config) );
  ESP_ERROR_CHECK( esp_wifi_start() );
}

#elif CONFIG_WIFI_MODE_TYPE_AP
static const char *TAG = "WIFI_AP";
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
  switch(event->event_id) {
		case SYSTEM_EVENT_AP_START:
			ESP_LOGI(TAG, "AP start event!\r\n");
			break;
		case SYSTEM_EVENT_AP_STACONNECTED:
			ESP_LOGI(TAG, "A station connected to ESP32 soft-AP!\r\n");
			break;
		case SYSTEM_EVENT_AP_STADISCONNECTED:
			ESP_LOGI(TAG, "A station disconnected from ESP32 soft-AP!\r\n");
			break;
		default:
			break;
    }

  return ESP_OK;
}


void user_wifi_config(void)
{
	ESP_LOGI(TAG, "AP config start\r\n");
	ESP_LOGI(TAG, "ESP32 soft-AP SSID: %s  PASSWORD: %s \r\n", CONFIG_AP_SSID, CONFIG_AP_PASSWORD);

	tcpip_adapter_init();

	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );

	wifi_config_t wifi_ap_config = {
      .ap = {
					.ssid = CONFIG_AP_SSID,
					.password = CONFIG_AP_PASSWORD,
					.ssid_len = strlen(CONFIG_AP_SSID),
					.channel = CONFIG_AP_CHANNEL,
					.max_connection = CONFIG_AP_MAX_CONNECT,
					.authmode = WIFI_AUTH_WPA_WPA2_PSK
      },
    };
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );
  ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_ap_config) );
  ESP_ERROR_CHECK( esp_wifi_start() );
}

#elif CONFIG_WIFI_MODE_TYPE_STA_AP
static const char *TAG = "WIFI_STA_AP";
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	wifi_ap_record_t my_wifi_info;

  switch(event->event_id) {
		case SYSTEM_EVENT_STA_START:
			ESP_LOGI(TAG, "STA start event!\r\n");
			ESP_ERROR_CHECK(esp_wifi_connect());
			break;
		case SYSTEM_EVENT_STA_GOT_IP:
			ESP_LOGI(TAG, "Get IP event!\r\n");
			ESP_LOGI(TAG, "ESP32 IP: %s !\r\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
			break;
		case SYSTEM_EVENT_STA_CONNECTED:
			ESP_LOGI(TAG, "Wifi STA connect event!\r\n");
			esp_wifi_sta_get_ap_info(&my_wifi_info);
			ESP_LOGI(TAG, "Connect to : %s!\r\n", my_wifi_info.ssid);
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:
			ESP_LOGI(TAG, "Wifi STA disconnect event, reconnect!\r\n");
			ESP_ERROR_CHECK( esp_wifi_connect() );
			break;
		case SYSTEM_EVENT_AP_START:
			ESP_LOGI(TAG, "AP start event!\r\n");

			break;
		case SYSTEM_EVENT_AP_STACONNECTED:
			ESP_LOGI(TAG, "A station connected to ESP32 soft-AP!\r\n");
			break;
		case SYSTEM_EVENT_AP_STADISCONNECTED:
			ESP_LOGI(TAG, "A station disconnected from ESP32 soft-AP!\r\n");
			break;
		default:
			break;
  }
  return ESP_OK;
}


void user_wifi_config(void)
{
	ESP_LOGI(TAG, "softAP+station config start!\r\n");
	ESP_LOGI(TAG, "connect to Wifi : %s \r\n", CONFIG_STA_SSID);
	ESP_LOGI(TAG, "ESP32 AP SSID: %s  PASSWORD: %s \r\n", CONFIG_AP_SSID, CONFIG_AP_PASSWORD);

	tcpip_adapter_init();

	ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );

  wifi_config_t wifi_sta_config = {
		.sta = {
			.ssid = CONFIG_STA_SSID,
			.password = CONFIG_STA_PASSWORD
		},
  };

	wifi_config_t wifi_ap_config = {
		.ap = {
      .ssid = CONFIG_AP_SSID,
			.password = CONFIG_AP_PASSWORD,
      .ssid_len = strlen(CONFIG_AP_SSID),
			.channel = CONFIG_AP_CHANNEL,
			.max_connection = CONFIG_AP_MAX_CONNECT,
      .authmode = WIFI_AUTH_WPA_WPA2_PSK
    },
  };

	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_APSTA) );
	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_sta_config) );
	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_ap_config) );
	ESP_ERROR_CHECK( esp_wifi_start() );
}
#endif
