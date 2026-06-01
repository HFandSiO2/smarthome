#include <stdio.h>
#include "gpio_def.h"
#include "esp_log.h"

static const char *TAG = "GPIO_DEF";

void gpio_init(void) {
    gpio_config_t sensor_io = {
        .pin_bit_mask = (1ULL << PIN_DHT11),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&sensor_io);

    ESP_LOGI(TAG, "GPIO initialized");
}