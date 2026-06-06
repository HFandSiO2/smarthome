#include "buzzer.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "BUZZER";
static bool s_on = false;

void buzzer_init(void) {
    /* 引脚已在 gpio_def 中配置 */
    gpio_set_level(PIN_BUZZER, 1);
    s_on = false;
    ESP_LOGI(TAG, "Buzzer ready");
}

void buzzer_on(void)  { gpio_set_level(PIN_BUZZER, 0); s_on = true; }
void buzzer_off(void) { gpio_set_level(PIN_BUZZER, 1); s_on = false; }
bool buzzer_is_on(void) { return s_on; }
