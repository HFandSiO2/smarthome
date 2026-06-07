#pragma once
#include <stdbool.h>
#include <time.h>

/**
 * @brief 初始化 NTP 并等待时间同步
 * @return true=同步成功, false=超时
 */
bool ntp_init(void);

/**
 * @brief 获取当前时间
 * @param[out] tm 时间结构体
 * @return true=时间有效, false=时间未同步
 */
bool ntp_get_time(struct tm *out);
