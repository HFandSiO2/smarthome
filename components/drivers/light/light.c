#include "light.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "LIGHT";
static bool s_on = false;

void light_init(void) {
    gpio_set_level(PIN_LIGHT, 0);
    s_on = false;
    ESP_LOGI(TAG, "Light ready");
}

void light_on(void)  { gpio_set_level(PIN_LIGHT, 1); s_on = true; }
void light_off(void) { gpio_set_level(PIN_LIGHT, 0); s_on = false; }
bool light_is_on(void) { return s_on; }
