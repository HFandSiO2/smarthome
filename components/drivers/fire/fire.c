#include "fire.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "FIRE";

void fire_init(void) {
    ESP_LOGI(TAG, "Fire sensor ready");
}

bool fire_detected(void) {
    /* 低电平 = 检测到火焰 */
    return gpio_get_level(PIN_FIRE) == 0;
}
