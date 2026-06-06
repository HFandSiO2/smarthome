/*
 * 参考自 https://github.com/UncleRus/esp-idf-lib (BSD Licensed)
 * 适配 ESP32-S3 + 本项目结构
 */
#include "dht11.h"
#include "config.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DHT11";

static gpio_num_t s_gpio = -1;

/* DHT11 时序基础间隔 (us) */
#define DHT_TIMER_INTERVAL  2
#define DHT_DATA_BITS       40

static portMUX_TYPE s_mux = portMUX_INITIALIZER_UNLOCKED;
#define PORT_ENTER_CRITICAL() portENTER_CRITICAL(&s_mux)
#define PORT_EXIT_CRITICAL()  portEXIT_CRITICAL(&s_mux)

/**
 * @brief 等待引脚到指定电平，记录耗时
 */
static esp_err_t dht_await_pin_state(int expected, uint32_t timeout_us,
                                     uint32_t *duration)
{
    gpio_set_direction(s_gpio, GPIO_MODE_INPUT);
    for (uint32_t i = 0; i < timeout_us; i += DHT_TIMER_INTERVAL) {
        esp_rom_delay_us(DHT_TIMER_INTERVAL);
        if (gpio_get_level(s_gpio) == expected) {
            if (duration) *duration = i;
            return ESP_OK;
        }
    }
    return ESP_ERR_TIMEOUT;
}

esp_err_t dht11_read(dht11_data_t *out) {
    if (s_gpio < 0) {
        ESP_LOGE(TAG, "DHT11 not initialized");
        return ESP_FAIL;
    }
    if (!out) {
        ESP_LOGE(TAG, "NULL pointer");
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t data[5] = {0};

    /* 关中断，防止时序被任务切换打断 */
    PORT_ENTER_CRITICAL();

    /* === Phase A: 主机拉低 ≥ 18ms === */
    gpio_set_direction(s_gpio, GPIO_MODE_OUTPUT_OD);
    gpio_set_level(s_gpio, 0);
    esp_rom_delay_us(20000);
    gpio_set_level(s_gpio, 1);
    esp_rom_delay_us(40);

    /* === Phase B/C/D: 等待 DHT11 响应 === */
    if (dht_await_pin_state(0, 40, NULL) != ESP_OK)       goto err;
    if (dht_await_pin_state(1, 88, NULL) != ESP_OK)       goto err;
    if (dht_await_pin_state(0, 88, NULL) != ESP_OK)       goto err;

    /* === 读取 40 bit 数据 === */
    for (int i = 0; i < DHT_DATA_BITS; i++) {
        uint32_t low_dur, high_dur;

        if (dht_await_pin_state(1, 65, &low_dur) != ESP_OK)  goto err;
        if (dht_await_pin_state(0, 75, &high_dur) != ESP_OK) goto err;

        uint8_t b = i / 8;
        if (i % 8 == 0) data[b] = 0;
        /* 高电平持续时间 > 低电平持续时间 → bit "1" */
        data[b] |= (high_dur > low_dur) << (7 - (i % 8));
    }

    /* 恢复开漏输出 */
    gpio_set_direction(s_gpio, GPIO_MODE_OUTPUT_OD);
    gpio_set_level(s_gpio, 1);

    PORT_EXIT_CRITICAL();

    /* === 校验和 === */
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if ((checksum & 0xFF) != data[4]) {
        ESP_LOGW(TAG, "Checksum error: %02X != %02X", checksum & 0xFF, data[4]);
        return ESP_FAIL;
    }

    /* === 解析 === */
    out->humidity    = (float)data[0];
    out->temperature = (float)data[2];

    ESP_LOGD(TAG, "T=%.1f°C  H=%.1f%%", out->temperature, out->humidity);
    return ESP_OK;

err:
    gpio_set_direction(s_gpio, GPIO_MODE_OUTPUT_OD);
    gpio_set_level(s_gpio, 1);
    PORT_EXIT_CRITICAL();
    return ESP_FAIL;
}
