#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include <button_handle/button_handle.h>
#include <led_handle/led_handle.h>

void app_main()
{
    QueueHandle_t mode_queue = xQueueCreate(5, sizeof(int));
    static button_config_t btn_config;
    btn_config.button_pin = 0;
    btn_config.mode_queue = mode_queue;
    btn_config.TAG = "btn_0";

    button_start_task(&btn_config);

    static led_config_t led_config;
    led_config.led_pin = 21;
    led_config.mode_queue = mode_queue;
    led_config.tag = "led_21";
    led_start_task(&led_config);
}
