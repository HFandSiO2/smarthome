#include "pir.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "PIR";

void pir_init(void) {
    ESP_LOGI(TAG, "PIR ready");
}

bool pir_detected(void) {
    /* 高电平 = 检测到人体 */
    return gpio_get_level(PIN_PIR) == 1;
}
