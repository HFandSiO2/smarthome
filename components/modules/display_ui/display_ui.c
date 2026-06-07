#include "display_ui.h"
#include "ssd1306.h"
#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "DISPLAY_UI";

void display_ui_init(void) {
    /* OLED 已在 system_init 中通过 oled_init() 初始化，此处直接清屏 */
    ssd1306_clear();
    ssd1306_draw_string(0, 0, "Smart Home");
    ssd1306_refresh();
    ESP_LOGI(TAG, "Display UI ready");
}

void display_ui_show_time(struct tm *tm) {
    char line[32];
    ssd1306_clear();

    strftime(line, sizeof(line), "%Y-%m-%d", tm);
    ssd1306_draw_string(0, 0, line);

    strftime(line, sizeof(line), "%H:%M:%S", tm);
    ssd1306_draw_string(24, 24, line);

    ssd1306_refresh();
}

void display_ui_show_sensor(float temperature, float humidity) {
    char line[32];

    ssd1306_clear();

    snprintf(line, sizeof(line), "Temp: %.1f C", temperature);
    ssd1306_draw_string(0, 0, line);

    snprintf(line, sizeof(line), "Hum:  %.1f %%", humidity);
    ssd1306_draw_string(0, 16, line);

    ssd1306_refresh();
}

void display_ui_show_menu(int menu, int setting_mode,
                          int start_hour, int end_hour) {
    char line[32];
    ssd1306_clear();

    if (menu == 0) {
        /* 菜单 0：传感器数据 — 由外部调用 show_sensor 刷新 */
        ssd1306_draw_string(0, 0, "< Sensor Data >");
    } else if (menu == 1) {
        /* 菜单 1：夜间模式设置 */
        ssd1306_draw_string(0, 0, "< Night Mode >");

        snprintf(line, sizeof(line), "Start: %02d:00", start_hour);
        ssd1306_draw_string(0, 16, line);
        snprintf(line, sizeof(line), "End:   %02d:00", end_hour);
        ssd1306_draw_string(0, 32, line);

        if (setting_mode == 1) ssd1306_draw_string(80, 16, "<");
        if (setting_mode == 2) ssd1306_draw_string(80, 32, "<");
    }

    ssd1306_refresh();
}
