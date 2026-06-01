#pragma once


typedef struct {
    float temperature;
    float humidity;
    float smoke_voltage;

    bool rain_detected;
    bool fire_detected;
    bool human_detected;

    bool fan_on;
    bool light_on;
    bool buzzer_on;
    bool window_open;

    uint8_t menu;
    bool night_mode;
}sys_state_t;


/**
 * @brief Initialize all system modules (SSD1306, etc.)
 */
void sys_init(void);
