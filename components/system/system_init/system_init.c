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

    vTaskDelay(pdMS_TO_TICKS(1000)); // 等待传感器稳定
    dht11_data_t dht;
    if (dht11_read(&dht) == ESP_OK) {
        ESP_LOGI(TAG, "DHT11: %.1f°C %.1f%%", dht.temperature, dht.humidity);
        char line[32];
        snprintf(line, sizeof(line), "Temp: %.1f C", dht.temperature);
        ssd1306_draw_string(0, 0, line);
        snprintf(line, sizeof(line), "Hum:  %.1f %%", dht.humidity);
        ssd1306_draw_string(0, 16, line);
        ssd1306_refresh();
    }
}
