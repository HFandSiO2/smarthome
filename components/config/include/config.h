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
#define PIN_DHT11       GPIO_NUM_42      /* DHT11 温湿度               */
#define PIN_RAIN        GPIO_NUM_47       /* 雨滴传感器 (数字量)        */
#define PIN_FIRE        GPIO_NUM_48       /* 火焰传感器 (数字量)        */
#define PIN_PIR         GPIO_NUM_21      /* 人体红外传感器 HC-SR501   */

/* ==================== ADC sensor ==================== */
#define PIN_MQ2         GPIO_NUM_43       /* MQ-2 烟雾传感器 (ADC1_CH4)*/
#define MQ2_ADC_CHANNEL GPIO_NUM_1
#define MQ2_ADC_ATTEN   GPIO_NUM_2

/* ==================== Actuator pins ==================== */
#define PIN_BUZZER      GPIO_NUM_40      /* 有源蜂鸣器                 */
#define PIN_FAN         GPIO_NUM_21      /* 风扇继电器                 */
#define PIN_LIGHT       GPIO_NUM_47      /* 灯光继电器                 */
#define PIN_WINDOW      GPIO_NUM_45      /* 窗户舵机 (PWM)            */

/* ==================== Stepper motor ==================== */
#define PIN_STEP_IN1    GPIO_NUM_9
#define PIN_STEP_IN2    GPIO_NUM_10
#define PIN_STEP_IN3    GPIO_NUM_11
#define PIN_STEP_IN4    GPIO_NUM_12

/* ==================== Button matrix (2x2) ==================== */
#define PIN_BTN_ROW1    GPIO_NUM_35
#define PIN_BTN_ROW2    GPIO_NUM_36
#define PIN_BTN_COL1    GPIO_NUM_37
#define PIN_BTN_COL2    GPIO_NUM_38

/* ==================== WiFi ==================== */
#define WIFI_SSID       "FreeWiFi"
#define WIFI_PASS       "12312344"

/* ==================== NTP ==================== */
#define NTP_SERVER      "ntp1.aliyun.com"
#define NTP_TZ          "CST-8"
