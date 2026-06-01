#include <stdio.h>
#include "gpio_def.h"
#include "esp_log.h"
#include "hw_config.h"

static const char *TAG = "GPIO_DEF";

void gpio_init(gpio_num_t gpio, gpio_type_t type, bool pull_up)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << gpio),
        .intr_type = GPIO_INTR_DISABLE,
    };

    if (type == GPIO_OUTPUT) {
        cfg.mode = GPIO_MODE_OUTPUT;
        cfg.pull_up_en = 0;
        cfg.pull_down_en = 0;
    } else {
        cfg.mode = GPIO_MODE_INPUT;
        cfg.pull_up_en = pull_up ? 1 : 0;
        cfg.pull_down_en = 0;
    }

    gpio_config(&cfg);
    ESP_LOGI(TAG, "GPIO %d initialized as %s with pull-up %s", gpio,
             (type == GPIO_OUTPUT) ? "OUTPUT" : "INPUT",
             pull_up ? "ENABLED" : "DISABLED");
}

