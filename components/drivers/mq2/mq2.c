#include "mq2.h"
#include "config.h"
#include "esp_log.h"
#include "esp_adc/adc_continuous.h"

static const char *TAG = "MQ2";
static adc_continuous_handle_t s_adc = NULL;

void mq2_init(void) {
    adc_continuous_handle_cfg_t adc_cfg = {
        .max_store_buf_size = 1024,
        .conv_frame_size = 256,
        .flags = {.flush_pool = 1},
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_cfg, &s_adc));

    adc_digi_pattern_config_t pattern = {
        .atten = ADC_ATTEN_DB_12,
        .bit_width = SOC_ADC_DIGI_MAX_BITWIDTH,
        .channel = MQ2_ADC_CHANNEL,
        .unit = ADC_UNIT_1,
    };
    adc_continuous_config_t cont_cfg = {
        .pattern_num = 1,
        .adc_pattern = &pattern,
        .sample_freq_hz = 1000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
    };
    ESP_ERROR_CHECK(adc_continuous_config(s_adc, &cont_cfg));
    ESP_ERROR_CHECK(adc_continuous_start(s_adc));
    ESP_LOGI(TAG, "MQ-2 ready");
}

float mq2_read_voltage(void) {
    uint8_t buf[256];
    uint32_t len = 0;
    uint32_t sum = 0, cnt = 0;

    if (adc_continuous_read(s_adc, buf, sizeof(buf), &len, ADC_MAX_DELAY) == ESP_OK) {
        for (int i = 0; i < len; i += SOC_ADC_DIGI_RESULT_BYTES) {
            adc_digi_output_data_t *d = (adc_digi_output_data_t *)&buf[i];
            if (d->type2.channel == MQ2_ADC_CHANNEL) {
                sum += d->type2.data;
                cnt++;
            }
        }
    }
    float avg = (cnt > 0) ? (float)sum / cnt : 0;
    return 5.0f * avg / 4096.0f;
}
