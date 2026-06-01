#pragma once
#include "driver/gpio.h"
#include "hal/adc_types.h"

/**
 * @brief GPIO type (input or output)
 */
typedef enum {
    GPIO_OUTPUT,        /**< Output mode */
    GPIO_INPUT,         /**< Input mode */
} gpio_type_t;

/**
 * @brief Initialize a GPIO pin
 *
 * @param gpio     GPIO number
 * @param type     GPIO_OUTPUT or GPIO_INPUT
 * @param pull_up  Enable pull-up (only for input mode)
 */
void gpio_init(gpio_num_t gpio, gpio_type_t type, bool pull_up);