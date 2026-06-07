#include "control.h"
#include "mq2.h"
#include "raindrop.h"
#include "fire.h"
#include "pir.h"
#include "dht11.h"
#include "buzzer.h"
#include "fan.h"
#include "light.h"
#include "stepper.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static const char *TAG = "CONTROL";

static SemaphoreHandle_t s_stepper_mutex = NULL;

/* ---- 互斥标志 ---- */
static bool s_smoke_alarm   = false;   /* 烟雾报警激活中 */
static bool s_stepper_by_mq2 = false;   /* 步进电机被 MQ2 占用 */

/* ================ 烟雾监控 ================ */
static void task_mq2(void *arg) {
    while (1) {
        float volt = mq2_read_voltage();
        ESP_LOGI(TAG, "Smoke: %.2fV", volt);

        if (volt >= 3.5f) {
            s_smoke_alarm = true;
            buzzer_on();
            fan_on();

            if (!s_stepper_by_mq2) {
                s_stepper_by_mq2 = true;
                if (xSemaphoreTake(s_stepper_mutex, portMAX_DELAY) == pdTRUE) {
                    stepper_rotate(128, false);   /* 关窗 */
                    xSemaphoreGive(s_stepper_mutex);
                }
            }
        } else {
            s_smoke_alarm = false;
            s_stepper_by_mq2 = false;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* ================ 雨滴监控 ================ */
static void task_raindrop(void *arg) {
    while (1) {
        /* MQ2 占用步进电机时，雨滴不操作窗户 */
        if (!s_stepper_by_mq2) {
            bool wet = raindrop_is_wet();

            if (wet) {
                /* 检测到雨 → 关窗 */
                if (xSemaphoreTake(s_stepper_mutex, portMAX_DELAY) == pdTRUE) {
                    stepper_rotate(128, true);    /* 关窗 */
                    xSemaphoreGive(s_stepper_mutex);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* ================ 火焰监控 ================ */
static void task_fire(void *arg) {
    while (1) {
        if (fire_detected()) {
            buzzer_on();
        } else if (!s_smoke_alarm) {
            buzzer_off();
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* ================ 人体感应灯 ================ */
static void task_pir(void *arg) {
    uint8_t delay_cnt = 0;

    while (1) {
        if (pir_detected()) {
            light_on();
            delay_cnt = 0;
        } else {
            delay_cnt++;
            if (delay_cnt >= 5) {   /* 无人约 5s 后关灯 */
                delay_cnt = 0;
                light_off();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* ================ 温控风扇 ================ */
static void task_temp_control(void *arg) {
    while (1) {
        if (!s_smoke_alarm) {
            dht11_data_t dht;
            if (dht11_read(&dht) == ESP_OK) {
                if (dht.temperature > 26.0f) {
                    fan_on();
                } else {
                    fan_off();
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void control_init(void) {
    s_stepper_mutex = xSemaphoreCreateMutex();

    xTaskCreate(task_mq2,           "ctrl_mq2",    3072, NULL, 5, NULL);
    xTaskCreate(task_raindrop,      "ctrl_rain",   2048, NULL, 5, NULL);
    xTaskCreate(task_fire,          "ctrl_fire",   2048, NULL, 5, NULL);
    xTaskCreate(task_pir,           "ctrl_pir",    2048, NULL, 5, NULL);
    xTaskCreate(task_temp_control,  "ctrl_temp",   3072, NULL, 4, NULL);

    ESP_LOGI(TAG, "Control module started");
}
