idf_component_register(SRCS "control.c"
                    INCLUDE_DIRS "include"
                    REQUIRES mq2 raindrop fire pir dht11 buzzer fan light stepper)
