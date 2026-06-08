#include "fan.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "FAN";
static bool s_on = false;

void fan_init(void) {
    s_on = false;
    ESP_LOGI(TAG, "Fan ready");
}

void fan_on(void)  { gpio_set_level(PIN_FAN, 1); s_on = true; }
void fan_off(void) { gpio_set_level(PIN_FAN, 0); s_on = false; }
bool fan_is_on(void) { return s_on; }
