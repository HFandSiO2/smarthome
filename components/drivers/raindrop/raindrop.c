#include "raindrop.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "RAINDROP";

void raindrop_init(void) {
    ESP_LOGI(TAG, "Raindrop ready");
}

bool raindrop_is_wet(void) {
    /* 低电平 = 检测到雨滴 */
    return gpio_get_level(PIN_RAIN) == 0;
}
