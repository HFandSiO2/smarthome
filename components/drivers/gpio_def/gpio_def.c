#include <stdio.h>
#include "gpio_def.h"
#include "esp_log.h"
#include "config.h"

static const char *TAG = "GPIO_DEF";

void gpio_init(void) {
    /* ==================== DHT11 ==================== */
    gpio_set_direction(PIN_DHT11, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(PIN_DHT11, GPIO_PULLUP_ONLY);
    gpio_set_level(PIN_DHT11, 1);

    ESP_LOGI(TAG, "GPIO initialized");
}

