idf_component_register(SRCS "system_init.c"
                    INCLUDE_DIRS "include"
                    REQUIRES gpio_def spi_bus ssd1306 dht11 display_ui menu wifi ntp control config)
