#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    /* ---- sensor data ---- */
    float temperature;
    float humidity;
    float smoke_voltage;

    bool rain_detected;
    bool fire_detected;
    bool human_detected;

    /* ---- actuator state ---- */
    bool fan_on;
    bool light_on;
    bool buzzer_on;
    bool window_open;

    /* ---- UI / system ---- */
    uint8_t menu;
    bool night_mode;
} sys_state_t;

extern sys_state_t sys;
