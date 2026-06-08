idf_component_register(SRCS "dht11.c"
                    INCLUDE_DIRS "include"
                    REQUIRES config esp_driver_gpio)