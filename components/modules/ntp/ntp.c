#include "ntp.h"
#include "config.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "NTP";
static bool s_synced = false;

void ntp_sync_cb(struct timeval *tv) {
    s_synced = true;
    ESP_LOGI(TAG, "Time synchronized");
}

bool ntp_init(void) {
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, NTP_SERVER);
    esp_sntp_set_time_sync_notification_cb(ntp_sync_cb);
    esp_sntp_init();

    setenv("TZ", NTP_TZ, 1);
    tzset();

    /* 等待同步，最多 20 次 × 500ms = 10s */
    time_t now = 0;
    struct tm tm = {0};
    int retry = 20;
    while (retry-- > 0 && tm.tm_year < (2024 - 1900)) {
        vTaskDelay(pdMS_TO_TICKS(500));
        time(&now);
        localtime_r(&now, &tm);
    }

    if (tm.tm_year >= (2024 - 1900)) {
        ESP_LOGI(TAG, "NTP sync OK: %s", asctime(&tm));
        return true;
    } else {
        ESP_LOGW(TAG, "NTP sync timeout");
        return false;
    }
}

bool ntp_get_time(struct tm *out) {
    if (!s_synced) return false;
    time_t now = 0;
    time(&now);
    localtime_r(&now, out);
    return true;
}
