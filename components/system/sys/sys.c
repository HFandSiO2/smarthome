#include <stdio.h>
#include "sys.h"
#include "esp_log.h"
#include "gpio_def.h"
#include "hw_config.h"
#include "display.h"
#include "dht11.h"

static const char *TAG = "SYS";

void sys_init(void) {
    /* 基础 GPIO 初始化 */
    gpio_init(PIN_DHT11, GPIO_INPUT, true);

    /* 各模块初始化 */
    display_init();     /* SPI 总线 + SSD1306 OLED 显示 */
    dht11_init(PIN_DHT11);

    ESP_LOGI(TAG, "All modules initialized");
}
