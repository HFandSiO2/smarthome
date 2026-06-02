#include "sys_state.h"

sys_state_t sys = {
    .temperature = 0.0f,
    .humidity = 0.0f,
    .smoke_voltage = 0.0f,

    .rain_detected = false,
    .fire_detected = false,
    .human_detected = false,

    .fan_on = false,
    .light_on = false,
    .buzzer_on = false,
    .window_open = false,

    .menu = 0,
    .night_mode = false,
};