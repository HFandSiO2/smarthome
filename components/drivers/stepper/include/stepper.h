#pragma once
#include <stdbool.h>
#include "esp_err.h"

void stepper_init(void);
void stepper_rotate(int steps, bool clockwise);
