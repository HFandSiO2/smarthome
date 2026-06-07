#pragma once
#include <stdint.h>

/** 按键 ID */
typedef enum {
    BTN_UP     = 0,   /* 上一项 */
    BTN_DOWN   = 1,   /* 下一项 */
    BTN_CONFIRM= 2,   /* 确认/退出 */
    BTN_BACK   = 3,   /* 返回/进入设置 */
} button_id_t;

/**
 * @brief 初始化 2x2 矩阵键盘
 */
void button_init(void);

/**
 * @brief 非阻塞扫描，返回当前按下的按键 ID
 * @return 按键 ID，无按键返回 -1
 */
int button_scan(void);
