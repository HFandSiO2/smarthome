#include <stdio.h>
#include "gpio_def.h"
#include "esp_log.h"

static const char *TAG = "GPIO_DEF";

void gpio_init(void) {
    /* ---------- 传感器输入引脚 ---------- */
    gpio_config_t sensor_io = {
        .pin_bit_mask = (1ULL << PIN_DHT11) |
                        (1ULL << PIN_RAIN)  |
                        (1ULL << PIN_FIRE)  |
                        (1ULL << PIN_PIR),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&sensor_io);

    /* DHT11 需要开漏输出模式 */
    gpio_set_direction(PIN_DHT11, GPIO_MODE_INPUT_OUTPUT_OD);

    /* ---------- 执行器输出引脚 ---------- */
    gpio_config_t actuator_io = {
        .pin_bit_mask = (1ULL << PIN_FAN)    |
                        (1ULL << PIN_LIGHT)  |
                        (1ULL << PIN_BUZZER),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&actuator_io);

    /* 初始状态：全部关闭 */
    gpio_set_level(PIN_FAN,    0);
    gpio_set_level(PIN_LIGHT,  0);
    gpio_set_level(PIN_BUZZER, 0);

    /* 窗户舵机由 LEDC (PWM) 模块独立初始化，此处只设置方向 */
    gpio_set_direction(PIN_WINDOW, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "GPIO initialized");
}