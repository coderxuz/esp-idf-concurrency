#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include <button_handle/button_handle.h>
#include <led_handle/led_handle.h>

static int MODE = 0;

void app_main()
{
    static button_config_t btn_config;
    btn_config.button_pin = 0;
    btn_config.mode = &MODE;
    btn_config.TAG = "btn_0";

    button_start_task(&btn_config);

    static led_config_t led_config;
    led_config.led_pin = 21;
    led_config.mode = &MODE;
    led_config.tag = "led_21";
    led_start_task(&led_config);
}
