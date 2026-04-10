idf_component_register(SRCS "button_handle/button_handle.c" "led_handle/led_handle.c" "wifi_handle/wifi_handle.c" "server_handle/server_handle.c" "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES led_strip)