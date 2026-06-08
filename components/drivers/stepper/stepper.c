#include "stepper.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* 8 拍半步驱动序列 */

static const char *TAG = "STEPPER";

/* 8 拍半步驱动序列 */
static const int s_seq[8][4] = {
    {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,1,1,0},
    {0,0,1,0}, {0,0,1,1}, {0,0,0,1}, {1,0,0,1},
};

static const gpio_num_t s_pins[4] = {
    PIN_STEP_IN1, PIN_STEP_IN2, PIN_STEP_IN3, PIN_STEP_IN4
};

void stepper_init(void) {
    ESP_LOGI(TAG, "Stepper ready");
}

void stepper_rotate(int steps, bool clockwise) {
    for (int step = 0; step < steps; step++) {
        for (int i = 0; i < 8; i++) {
            int idx = clockwise ? i : 7 - i;
            for (int p = 0; p < 4; p++)
                gpio_set_level(s_pins[p], s_seq[idx][p]);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}
