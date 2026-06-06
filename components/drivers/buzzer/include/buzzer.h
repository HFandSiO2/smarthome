#pragma once
#include <stdbool.h>
#include "esp_err.h"

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
bool buzzer_is_on(void);
