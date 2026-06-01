#include "dht11.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DHT11";

static int s_gpio = -1;

/* DHT11 时序参数 (us) */
#define DHT11_START_LOW     18000   /* 主机拉低 ≥ 18ms */
#define DHT11_START_HIGH    30      /* 主机拉高 20-40us */
#define DHT11_RESP_HIGH     80      /* 从机响应高电平 80us */
#define DHT11_RESP_LOW      80      /* 从机响应低电平 80us */
#define DHT11_BIT_HALF      50      /* 数据位半周期 */

void dht11_init(int gpio_num) {
    s_gpio = gpio_num;
    gpio_set_direction(s_gpio, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_level(s_gpio, 1);  /* 空闲时拉高 */
    ESP_LOGI(TAG, "DHT11 initialized on GPIO %d", s_gpio);
}

/**
 * @brief 等待引脚电平，超时返回 false
 */
static inline bool dht11_wait_level(int level, uint32_t timeout_us) {
    uint32_t start = (uint32_t)esp_timer_get_time();
    while ((uint32_t)esp_timer_get_time() - start < timeout_us) {
        if (gpio_get_level(s_gpio) == level) return true;
    }
    return false;
}

esp_err_t dht11_read(dht11_data_t *out) {
    if (s_gpio < 0) {
        ESP_LOGE(TAG, "DHT11 not initialized");
        return ESP_FAIL;
    }

    uint8_t data[5] = {0};

    /* === 发送起始信号 === */
    gpio_set_level(s_gpio, 0);
    ets_delay_us(DHT11_START_LOW);
    gpio_set_level(s_gpio, 1);
    ets_delay_us(DHT11_START_HIGH);
    gpio_set_direction(s_gpio, GPIO_MODE_INPUT);  /* 切换为输入 */

    /* === 等待从机响应 === */
    if (!dht11_wait_level(0, 100) || !dht11_wait_level(1, 100)) {
        ESP_LOGW(TAG, "DHT11 no response");
        gpio_set_direction(s_gpio, GPIO_MODE_INPUT_OUTPUT_OD);
        gpio_set_level(s_gpio, 1);
        return ESP_FAIL;
    }

    /* === 读取 40 bit 数据 === */
    for (int i = 0; i < 40; i++) {
        /* 等待低电平结束 (50us) */
        if (!dht11_wait_level(1, 100)) {
            ESP_LOGW(TAG, "DHT11 timeout bit %d", i);
            gpio_set_direction(s_gpio, GPIO_MODE_INPUT_OUTPUT_OD);
            gpio_set_level(s_gpio, 1);
            return ESP_FAIL;
        }
        /* 延时半周期后采样 */
        ets_delay_us(DHT11_BIT_HALF);
        int level = gpio_get_level(s_gpio);
        data[i / 8] = (data[i / 8] << 1) | (level & 1);
        /* 等待位结束 */
        dht11_wait_level(0, 100);
    }

    /* 恢复开漏输出 */
    gpio_set_direction(s_gpio, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_level(s_gpio, 1);

    /* === 校验和 === */
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if ((checksum & 0xFF) != data[4]) {
        ESP_LOGW(TAG, "DHT11 checksum error: %02X != %02X",
                 checksum & 0xFF, data[4]);
        return ESP_FAIL;
    }

    /* === 解析 === */
    out->humidity    = (float)data[0];
    out->temperature = (float)data[2];

    ESP_LOGD(TAG, "T=%.1f°C  H=%.1f%%", out->temperature, out->humidity);
    return ESP_OK;
}
