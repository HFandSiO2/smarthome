#pragma once

/**
 * @brief Display UI module — OLED 数据显示与菜单
 * 底层使用 ssd1306 驱动，本层只负责排版布局。
 */

void display_ui_init(void);

/**
 * @brief 显示传感器数据（温湿度）
 */
void display_ui_show_sensor(float temperature, float humidity);

/**
 * @brief 显示菜单页面
 * @param menu   菜单索引 (0=传感器, 1=设置)
 * @param setting_mode 0=浏览, 1=编辑开始时间, 2=编辑结束时间
 * @param start_hour  夜间模式开始小时
 * @param end_hour    夜间模式结束小时
 */
void display_ui_show_menu(int menu, int setting_mode,
                          int start_hour, int end_hour);
