#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64

typedef void* ssd1306_handle_t;

/**
 * @brief Create SSD1306 device on pre-initialized SPI bus
 *
 * @param cs_gpio   CS GPIO number
 * @param dc_gpio   DC GPIO number
 * @param rst_gpio  RES GPIO number
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t ssd1306_init(int cs_gpio, int dc_gpio, int rst_gpio);

/**
 * @brief Initialize OLED using pins from config.h (no-arg convenience)
 */
void oled_init(void);

/**
 * @brief Clear framebuffer
 */
void ssd1306_clear(void);

/**
 * @brief Draw a pixel
 */
void ssd1306_draw_pixel(int x, int y, bool on);

/**
 * @brief Draw a string
 */
void ssd1306_draw_string(int x, int y, const char *str);

/**
 * @brief Refresh display (send framebuffer to screen)
 */
void ssd1306_refresh(void);
