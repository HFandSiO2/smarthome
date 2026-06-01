#pragma once
#include "esp_err.h"

/**
 * @brief SPI bus handle for external use
 */
typedef int spi_bus_handle_t;
#define SPI_BUS_HOST SPI2_HOST

/**
 * @brief Initialize SPI2 host bus (shared by all SPI devices)
 *
 * @param mosi_gpio MOSI GPIO number
 * @param sclk_gpio SCLK GPIO number
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_STATE Already initialized
 *     - ESP_FAIL Failed
 */
esp_err_t spi_bus_init(int mosi_gpio, int sclk_gpio);

/**
 * @brief Get SPI bus host handle
 * @return spi_bus_handle_t
 */
spi_bus_handle_t spi_bus_get_host(void);
