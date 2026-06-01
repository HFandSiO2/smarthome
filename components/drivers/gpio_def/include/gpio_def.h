#pragma once
#include "driver/gpio.h"
#include "hal/adc_types.h"

/* ==================== SPI pins for SSD1306 OLED ==================== */
#define PIN_MOSI        GPIO_NUM_15
#define PIN_SCLK        GPIO_NUM_7
#define PIN_CS          GPIO_NUM_18
#define PIN_DC          GPIO_NUM_17
#define PIN_RES         GPIO_NUM_16

/* ==================== Sensor pins ==================== */
#define PIN_DHT11       GPIO_NUM_42      /* DHT11 温湿度传感器      */
// #define PIN_SMOKE_ADC   GPIO_NUM_4       /* MQ-2 烟雾传感器 (ADC)  */
// #define PIN_RAIN        GPIO_NUM_5       /* 雨滴传感器 (数字量)     */
// #define PIN_FIRE        GPIO_NUM_6       /* 火焰传感器 (数字量)     */
// #define PIN_PIR         GPIO_NUM_14      /* 人体红外传感器 (数字量) */

// /* ==================== Actuator pins ==================== */
// #define PIN_FAN         GPIO_NUM_21      /* 风扇继电器              */
// #define PIN_LIGHT       GPIO_NUM_47      /* 灯光继电器              */
// #define PIN_BUZZER      GPIO_NUM_48      /* 蜂鸣器                  */
// #define PIN_WINDOW      GPIO_NUM_45      /* 窗户舵机 (PWM)         */

// /* ==================== ADC config ==================== */
// #define SMOKE_ADC_CHANNEL   ADC_CHANNEL_3    /* GPIO4 -> ADC1_CH3 */
// #define SMOKE_ADC_ATTEN     ADC_ATTEN_DB_12

/**
 * @brief Initialize all GPIO pins (sensors input, actuators output)
 */
void gpio_init(void);