#include "display.h"
#include "ssd1306.h"
#include "spi_bus.h"
#include "hw_config.h"
#include "esp_log.h"

static const char *TAG = "DISPLAY";

void display_init(void) {
    /* SPI 总线初始化（可挂载多个 SPI 设备） */
    ESP_ERROR_CHECK(spi_bus_init(PIN_MOSI, PIN_SCLK));

    /* SSD1306 OLED 初始化 */
    ESP_ERROR_CHECK(ssd1306_init(PIN_CS, PIN_DC, PIN_RES));
    ESP_LOGI(TAG, "SSD1306 initialized");
}
