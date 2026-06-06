#include <stdio.h>
#include "gpio_def.h"
#include "esp_log.h"
#include "config.h"

static const char *TAG = "GPIO_DEF";

void gpio_init(void) {
    /* ==================== DHT11 (开漏输出) ==================== */
    gpio_set_direction(PIN_DHT11, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(PIN_DHT11, GPIO_PULLUP_ONLY);
    gpio_set_level(PIN_DHT11, 1);

    /* ==================== 传感器输入 ==================== */
    gpio_config_t sensor_in = {
        .pin_bit_mask = (1ULL << PIN_RAIN) |
                        (1ULL << PIN_FIRE) |
                        (1ULL << PIN_PIR),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&sensor_in);

    /* ==================== 执行器输出 ==================== */
    gpio_config_t act_out = {
        .pin_bit_mask = (1ULL << PIN_BUZZER) |
                        (1ULL << PIN_FAN)    |
                        (1ULL << PIN_LIGHT),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&act_out);

    /* 初始状态：全部关闭 */
    gpio_set_level(PIN_BUZZER, 1);  /* 蜂鸣器高电平=关 */
    gpio_set_level(PIN_FAN,    0);
    gpio_set_level(PIN_LIGHT,  0);

    /* ==================== 步进电机 ==================== */
    gpio_config_t step_out = {
        .pin_bit_mask = (1ULL << PIN_STEP_IN1) |
                        (1ULL << PIN_STEP_IN2) |
                        (1ULL << PIN_STEP_IN3) |
                        (1ULL << PIN_STEP_IN4),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&step_out);

    ESP_LOGI(TAG, "GPIO initialized");
}

