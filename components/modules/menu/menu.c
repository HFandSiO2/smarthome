#include "menu.h"
#include "button.h"
#include "display_ui.h"
#include "dht11.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MENU";

/* ---- 菜单状态 ---- */
static int s_menu         = 0;   /* 0=传感器, 1=设置 */
static int s_setting_mode = 0;   /* 0=浏览, 1=编辑 start, 2=编辑 end */
static int s_start_hour   = 18;  /* 夜间模式开始 18:00 */
static int s_end_hour     = 6;   /* 夜间模式结束 6:00 */
static int s_menu_max     = 1;   /* 菜单总数-1（暂时没有时间菜单）*/

/* ---- 按键回调 ---- */
static void on_btn_up(void) {
    if (s_menu == 1 && s_setting_mode != 0) {
        /* 设置模式：调整数值 */
        if (s_setting_mode == 1) s_start_hour = (s_start_hour - 1 + 24) % 24;
        if (s_setting_mode == 2) s_end_hour   = (s_end_hour   - 1 + 24) % 24;
    } else {
        s_menu = (s_menu == 0) ? s_menu_max : s_menu - 1;
        s_setting_mode = 0;
    }
    ESP_LOGI(TAG, "Menu:%d set:%d", s_menu, s_setting_mode);
}

static void on_btn_down(void) {
    if (s_menu == 1 && s_setting_mode != 0) {
        if (s_setting_mode == 1) s_start_hour = (s_start_hour + 1) % 24;
        if (s_setting_mode == 2) s_end_hour   = (s_end_hour   + 1) % 24;
    } else {
        s_menu = (s_menu + 1) % (s_menu_max + 1);
        s_setting_mode = 0;
    }
    ESP_LOGI(TAG, "Menu:%d set:%d", s_menu, s_setting_mode);
}

static void on_btn_confirm(void) {
    if (s_menu == 1) s_setting_mode = 0;
}

static void on_btn_back(void) {
    if (s_menu == 1) {
        if (s_setting_mode == 0)      s_setting_mode = 1;
        else if (s_setting_mode == 1) s_setting_mode = 2;
        else if (s_setting_mode == 2) s_setting_mode = 1;
    }
}

/* ---- 菜单刷新显示 ---- */
static void refresh_display(void) {
    if (s_menu == 0) {
        dht11_data_t dht;
        if (dht11_read(&dht) == ESP_OK) {
            display_ui_show_sensor(dht.temperature, dht.humidity);
        }
    } else {
        display_ui_show_menu(s_menu, s_setting_mode, s_start_hour, s_end_hour);
    }
}

/* ---- 按键扫描任务 ---- */
static void task_menu(void *arg) {
    while (1) {
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
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void menu_init(void) {
    button_init();
    refresh_display();
    xTaskCreate(task_menu, "menu", 3072, NULL, 4, NULL);
    ESP_LOGI(TAG, "Menu module started");
}
