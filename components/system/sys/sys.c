#include <stdio.h>
#include "sys.h"
#include "esp_log.h"
#include "ssd1306.h"
#include "gpio_def.h"

static const char *TAG = "SYS";

void sys_init(void) {
    gpio_init();

    ESP_ERROR_CHECK(ssd1306_init(PIN_MOSI, PIN_SCLK, PIN_CS, PIN_DC, PIN_RES));
    ESP_LOGI(TAG, "SSD1306 initialized");

}
