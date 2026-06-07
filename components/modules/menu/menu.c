#include "menu.h"
#include "button.h"
#include "display_ui.h"
#include "ssd1306.h"
#include "dht11.h"
#include "ntp.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MENU";

/* ---- 菜单状态 ---- */
static int s_menu         = 0;   /* 0=时间, 1=传感器, 2=设置 */
static int s_setting_mode = 0;   /* 0=浏览, 1=编辑 start, 2=编辑 end */
static int s_start_hour   = 18;
static int s_end_hour     = 6;
static int s_menu_max     = 2;

/* ---- 按键回调 ---- */
static void on_btn_up(void) {
    if (s_menu == 2 && s_setting_mode != 0) {
        if (s_setting_mode == 1) s_start_hour = (s_start_hour - 1 + 24) % 24;
        if (s_setting_mode == 2) s_end_hour   = (s_end_hour   - 1 + 24) % 24;
    } else {
        s_menu = (s_menu == 0) ? s_menu_max : s_menu - 1;
        s_setting_mode = 0;
    }
}

static void on_btn_down(void) {
    if (s_menu == 2 && s_setting_mode != 0) {
        if (s_setting_mode == 1) s_start_hour = (s_start_hour + 1) % 24;
        if (s_setting_mode == 2) s_end_hour   = (s_end_hour   + 1) % 24;
    } else {
        s_menu = (s_menu + 1) % (s_menu_max + 1);
        s_setting_mode = 0;
    }
}

static void on_btn_confirm(void) {
    if (s_menu == 2) s_setting_mode = 0;
}

static void on_btn_back(void) {
    if (s_menu == 2) {
        if (s_setting_mode == 0)      s_setting_mode = 1;
        else if (s_setting_mode == 1) s_setting_mode = 2;
        else if (s_setting_mode == 2) s_setting_mode = 1;
    }
}

/* ---- 菜单刷新 ---- */
static void refresh_display(void) {
    switch (s_menu) {
    case 0: {
        struct tm tm;
        if (ntp_get_time(&tm)) {
            display_ui_show_time(&tm);
        } else {
            ssd1306_clear();
            ssd1306_draw_string(0, 0, "Syncing time...");
            ssd1306_refresh();
        }
        break;
    }
    case 1: {
        dht11_data_t dht;
        if (dht11_read(&dht) == ESP_OK)
            display_ui_show_sensor(dht.temperature, dht.humidity);
        break;
    }
    case 2:
        display_ui_show_menu(s_menu, s_setting_mode, s_start_hour, s_end_hour);
        break;
    }
}

/* ---- 按键扫描 ---- */
static void task_menu(void *arg) {
    TickType_t last_refresh = 0;
    while (1) {
        /* 按键处理 */
        int key = button_scan();
        if (key >= 0) {
            switch (key) {
                case BTN_UP:      on_btn_up();      break;
                case BTN_DOWN:    on_btn_down();    break;
                case BTN_CONFIRM: on_btn_confirm(); break;
                case BTN_BACK:    on_btn_back();    break;
            }
            refresh_display();
        }
        /* 时间页面每秒刷新 */
        if (s_menu == 0 && xTaskGetTickCount() - last_refresh > pdMS_TO_TICKS(1000)) {
            last_refresh = xTaskGetTickCount();
            refresh_display();
        }
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void menu_init(void) {
    button_init();
    refresh_display();
    xTaskCreate(task_menu, "menu", 3072, NULL, 4, NULL);
    ESP_LOGI(TAG, "Menu module started");
}
