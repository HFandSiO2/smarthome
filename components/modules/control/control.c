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
#include "sys_state.h"
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
        sys.smoke_voltage = volt;
        ESP_LOGI(TAG, "Smoke: %.2fV", volt);

        if (volt >= 3.5f) {
            s_smoke_alarm = true;
            buzzer_on();
            sys.buzzer_on = true;
            fan_on();
            sys.fan_on = true;

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
            sys.buzzer_on = false;
            sys.fan_on = false;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* ================ 雨滴监控 ================ */
static void task_raindrop(void *arg) {
    while (1) {
        if (!s_stepper_by_mq2) {
            bool wet = raindrop_is_wet();
            sys.rain_detected = wet;

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
        bool fire = fire_detected();
        sys.fire_detected = fire;
        if (fire) {
            buzzer_on();
            sys.buzzer_on = true;
        } else if (!s_smoke_alarm) {
            buzzer_off();
            sys.buzzer_on = false;
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* ================ 人体感应灯 ================ */
static void task_pir(void *arg) {
    uint8_t delay_cnt = 0;

    while (1) {
        if (pir_detected()) {
            sys.human_detected = true;
            light_on();
            sys.light_on = true;
            delay_cnt = 0;
        } else {
            sys.human_detected = false;
            delay_cnt++;
            if (delay_cnt >= 5) {
                delay_cnt = 0;
                light_off();
                sys.light_on = false;
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
                sys.temperature = dht.temperature;
                sys.humidity    = dht.humidity;
                if (dht.temperature > 26.0f) {
                    fan_on();
                    sys.fan_on = true;
                } else {
                    fan_off();
                    sys.fan_on = false;
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
