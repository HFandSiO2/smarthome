#include "button.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "BUTTON";

/* 矩阵映射：行(输出)、列(输入) */
static const gpio_num_t s_rows[2] = { PIN_BTN_ROW1, PIN_BTN_ROW2 };
static const gpio_num_t s_cols[2] = { PIN_BTN_COL1, PIN_BTN_COL2 };

/* 按键去抖 */
static int s_last_key = -1;
static int s_stable   = -1;
static int s_count    = 0;

#define DEBOUNCE_MS 3   /* 连续读到相同值 3 次才算有效 */

void button_init(void) {
    /* 行：开漏输出，默认高电平 */
    for (int i = 0; i < 2; i++) {
        gpio_set_direction(s_rows[i], GPIO_MODE_OUTPUT_OD);
        gpio_set_level(s_rows[i], 1);
    }
    /* 列：输入，内部上拉 */
    for (int i = 0; i < 2; i++) {
        gpio_set_direction(s_cols[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(s_cols[i], GPIO_PULLUP_ONLY);
    }
    ESP_LOGI(TAG, "Matrix keypad ready");
}

int button_scan(void) {
    int pressed = -1;

    /* 第一步：全扫——所有行拉低，看是否有列被拉低 */
    for (int r = 0; r < 2; r++) gpio_set_level(s_rows[r], 0);
    vTaskDelay(pdMS_TO_TICKS(1));

    int any_col_low = 0;
    for (int c = 0; c < 2; c++) {
        if (gpio_get_level(s_cols[c]) == 0) { any_col_low = 1; break; }
    }

    if (any_col_low) {
        /* 第二步：逐行扫描定位按键 */
        for (int r = 0; r < 2; r++) {
            gpio_set_level(s_rows[r], 0);
            for (int rr = 0; rr < 2; rr++)
                if (rr != r) gpio_set_level(s_rows[rr], 1);
            vTaskDelay(pdMS_TO_TICKS(1));

            for (int c = 0; c < 2; c++) {
                if (gpio_get_level(s_cols[c]) == 0) {
                    pressed = r * 2 + c;
                    break;
                }
            }
            if (pressed >= 0) break;
        }
    }

    /* 恢复所有行为高电平 */
    for (int r = 0; r < 2; r++) gpio_set_level(s_rows[r], 1);

    /* 去抖 */
    if (pressed == s_last_key) {
        s_count++;
        if (s_count >= DEBOUNCE_MS && s_stable != pressed) {
            s_stable = pressed;
            s_count = 0;
            return pressed;
        }
    } else {
        s_last_key = pressed;
        s_count = 0;
    }
    return -1;
}
