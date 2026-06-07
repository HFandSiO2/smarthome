idf_component_register(SRCS "wifi.c"
                    INCLUDE_DIRS "include"
                    REQUIRES config nvs_flash esp_wifi esp_event esp_netif)
