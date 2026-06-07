#pragma once
#include <time.h>

/**
 * @brief Display UI module — OLED 数据显示与菜单
 * 底层使用 ssd1306 驱动，本层只负责排版布局。
 */

void display_ui_init(void);

/**
 * @brief 显示当前时间
 */
void display_ui_show_time(struct tm *tm);

/**
 * @brief 显示传感器数据（温湿度）
 */
void display_ui_show_sensor(float temperature, float humidity);

/**
 * @brief 显示菜单页面
 */
void display_ui_show_menu(int menu, int setting_mode,
                          int start_hour, int end_hour);
