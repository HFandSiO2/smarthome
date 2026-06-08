#include "system_init.h"
#include "esp_log.h"
#include "gpio_def.h"
#include "spi_bus.h"
#include "ssd1306.h"
#include "display_ui.h"
#include "menu.h"
#include "wifi.h"
#include "ntp.h"
#include "control.h"

static const char *TAG = "INIT";

void system_init(void) {
    gpio_init();
    spi_init();
    oled_init();
    display_ui_init();
    wifi_init();
    ntp_init();
    menu_init();
    control_init();

    ESP_LOGI(TAG, "All modules initialized");
}
