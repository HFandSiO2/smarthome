#include <stdio.h>
#include "system_init.h"
#include "esp_log.h"
#include "gpio_def.h"
#include "spi_bus.h"
#include "ssd1306.h"
#include "config.h"
#include "dht11.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "INIT";

void system_init(void) {
    /* 基础 GPIO 初始化 */
    gpio_init();
    spi_init();
    oled_init();
    dht11_init();
    ESP_LOGI(TAG, "All modules initialized");

}
