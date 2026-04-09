#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#include <led_handle/led_handle.h>

void led_task(void *pv)
{
    led_config_t *config = (led_config_t *)pv;
    gpio_reset_pin(config->led_pin);
    gpio_set_direction(config->led_pin, GPIO_MODE_OUTPUT);
    while (1)
    {
        if (*(config->mode) == 0)
        {
            gpio_set_level(config->led_pin, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        else
        {
            if (*(config->mode) == 1)
            {
                gpio_set_level(config->led_pin, 1);
                vTaskDelay(pdMS_TO_TICKS(500));
                gpio_set_level(config->led_pin, 0);
                vTaskDelay(pdMS_TO_TICKS(500));
            }
            if (*(config->mode) == 2)
            {
                gpio_set_level(config->led_pin, 1);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_set_level(config->led_pin, 0);
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }
    }
}
void led_start_task(led_config_t *config)
{
    xTaskCreate(led_task, "led", 2048, config, 5, NULL);
}