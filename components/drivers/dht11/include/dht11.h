#pragma once
#include <stdint.h>
#include "esp_err.h"

/**
 * @brief DHT11 传感器数据结构
 */
typedef struct {
    float temperature;      /**< 温度 (°C) */
    float humidity;         /**< 湿度 (%RH) */
} dht11_data_t;

/**
 * @brief 读取 DHT11 温湿度数据
 * @param[out] out  读取到的数据
 * @return
 *     - ESP_OK    读取成功
 *     - ESP_FAIL  读取失败（超时/校验错误）
 */
esp_err_t dht11_read(dht11_data_t *out);


