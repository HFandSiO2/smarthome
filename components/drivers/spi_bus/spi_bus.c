#include "spi_bus.h"
#include "driver/spi_master.h"
#include "esp_log.h"

static const char *TAG = "SPI_BUS";
static bool s_initialized = false;

esp_err_t spi_bus_init(int mosi_gpio, int sclk_gpio) {
    if (s_initialized) {
        ESP_LOGW(TAG, "SPI bus already initialized");
        return ESP_OK;
    }

    spi_bus_config_t buscfg = {
        .mosi_io_num = mosi_gpio,
        .miso_io_num = -1,
        .sclk_io_num = sclk_gpio,
        .max_transfer_sz = 0,
    };

    esp_err_t ret = spi_bus_initialize(SPI_BUS_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI bus init failed");
        return ret;
    }

    s_initialized = true;
    ESP_LOGI(TAG, "SPI bus initialized (MOSI=%d, SCLK=%d)", mosi_gpio, sclk_gpio);
    return ESP_OK;
}

spi_bus_handle_t spi_bus_get_host(void) {
    return SPI_BUS_HOST;
}
