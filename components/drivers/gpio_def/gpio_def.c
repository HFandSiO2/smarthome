#include "gpio_def.h"
#include "esp_log.h"
#include "config.h"

static const char *TAG = "GPIO_DEF";

void gpio_init(void) {
    /* DHT11 — 开漏输出，外部上拉 */
    const gpio_config_t dht11 = {
        .pin_bit_mask = (1ULL << PIN_DHT11),
        .mode         = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&dht11);
    gpio_set_level(PIN_DHT11, 1);

    /* 传感器输入 — 雨滴/火焰/人体红外 */
    const gpio_config_t sensor_in = {
        .pin_bit_mask = (1ULL << PIN_RAIN) | (1ULL << PIN_FIRE) | (1ULL << PIN_PIR),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&sensor_in);

    /* 执行器输出 — 蜂鸣器/风扇/灯光 */
    const gpio_config_t act_out = {
        .pin_bit_mask = (1ULL << PIN_BUZZER) | (1ULL << PIN_FAN) | (1ULL << PIN_LIGHT),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&act_out);
    gpio_set_level(PIN_BUZZER, 1);   /* 蜂鸣器高电平=关 */
    gpio_set_level(PIN_FAN,    0);
    gpio_set_level(PIN_LIGHT,  0);

    /* 步进电机引脚 */
    const gpio_config_t step_out = {
        .pin_bit_mask = (1ULL << PIN_STEP_IN1) | (1ULL << PIN_STEP_IN2)
                      | (1ULL << PIN_STEP_IN3) | (1ULL << PIN_STEP_IN4),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&step_out);

    ESP_LOGI(TAG, "GPIO initialized");
}

