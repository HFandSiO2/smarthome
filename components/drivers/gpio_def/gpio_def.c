#include <stdio.h>
#include "gpio_def.h"
#include "esp_log.h"
#include "config.h"

static const char *TAG = "GPIO_DEF";

void gpio_init(void) {
    /* 各模块自行初始化自己的引脚（dht11, ssd1306, spi_bus 等），
       此函数保留用于未来需要统一配置的引脚 */
    ESP_LOGI(TAG, "GPIO system ready");
}

