# SmartHome — ESP32-S3

基于 ESP32-S3 的智能家居控制系统。OLED 显示、WiFi 联网、NTP 校时、多传感器监测与自动控制。

## 架构

```text
config/        引脚/参数定义，不含逻辑          ← 配置不管活
drivers/       纯硬件操作，不知用途             ← 驱动不知态
 ├─ gpio_def   统一 GPIO 配置
 ├─ spi_bus    SPI 总线
 ├─ ssd1306    OLED 显示驱动
 ├─ dht11      DHT11 温湿度传感器
 ├─ mq2        MQ-2 烟雾传感器 (ADC)
 ├─ raindrop   雨滴传感器
 ├─ fire       火焰传感器
 ├─ pir        人体红外传感器
 ├─ buzzer     蜂鸣器
 ├─ fan        风扇继电器
 ├─ light      灯光继电器
 ├─ stepper    28BYJ-48 步进电机
 └─ button     2×2 矩阵键盘
modules/       业务逻辑、状态机、自动控制       ← 模块管逻辑
 ├─ display_ui OLED 排版布局
 ├─ menu       菜单切换 + 参数设置
 ├─ control    自动控制（5 个任务）
 ├─ wifi       WiFi 连接
 └─ ntp        NTP 时间同步
system_init/   按序初始化，不关心细节           ← 系统只组装
main/          入口，调 system_init()
```

## 引脚定义

见 `components/config/include/config.h`。

| 功能 | 引脚 | 类型 |
| ---- | ---- | ---- |
| OLED SPI MOSI | GPIO 15 | 输出 |
| OLED SPI SCLK | GPIO 7 | 输出 |
| OLED SPI CS | GPIO 18 | 输出 |
| OLED SPI DC | GPIO 17 | 输出 |
| OLED SPI RES | GPIO 16 | 输出 |
| DHT11 温湿度 | GPIO 42 | 开漏 |
| MQ-2 烟雾 | GPIO 1 (ADC1_CH0) | 输入 |
| 雨滴传感器 | GPIO 41 | 输入 |
| 火焰传感器 | GPIO 40 | 输入 |
| 人体红外 (PIR) | GPIO 20 | 输入 |
| 蜂鸣器 | GPIO 19 | 输出 |
| 风扇继电器 | GPIO 3 | 输出 |
| 灯光继电器 | GPIO 4 | 输出 |
| 步进电机 IN1-4 | GPIO 9/10/11/12 | 输出 |
| 按键矩阵 ROW1/2 | GPIO 35/36 | 输出 |
| 按键矩阵 COL1/2 | GPIO 37/38 | 输入上拉 |

## 菜单

| 页面 | 内容 |
| --- | ---- |
| 0 | 当前时间（NTP 同步） |
| 1 | 温湿度传感器数据 |
| 2 | 夜间模式设置（开始/结束小时） |

按键：UP/DOWN 切换菜单/调整数值，CONFIRM 退出设置，BACK 进入设置。

## 自动控制

| 任务 | 触发条件 | 动作 |
| ---- | ------ | ---- |
| MQ2 烟雾 | 电压 ≥ 3.5V | 蜂鸣器响、风扇开、步进电机关窗 |
| 雨滴 | 检测到雨 | 步进电机关窗 |
| 火焰 | 检测到火 | 蜂鸣器响 |
| 人体感应 (PIR) | 检测到人 | 开灯，5s 无人后关灯 |
| 温控风扇 | 温度 > 26°C | 风扇开 |

## WiFi 配置

在 `components/config/include/config.h` 中修改 `WIFI_SSID` 和 `WIFI_PASS`。

## NTP 配置

默认使用 `ntp1.aliyun.com`，时区 `CST-8`。可在 `config.h` 中修改 `NTP_SERVER` 和 `NTP_TZ`。
