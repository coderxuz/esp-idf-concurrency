#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>


#include <button_handle/button_handle.h>

void button_task(void *pv)
{
    button_config_t *config = (button_config_t *)pv;
    gpio_reset_pin(config->button_pin);
    gpio_set_direction(config->button_pin, GPIO_MODE_INPUT);
    gpio_pullup_en(config->button_pin);
    int button_last_state = 1;
    int local_mode = 0;
    while (1)
    {
        int button_level = gpio_get_level(config->button_pin);
        if (button_last_state == 1 && button_level == 0)
        {
            local_mode = (local_mode + 1) % 3;
            xQueueSend(config->mode_queue, &local_mode, 0);
            ESP_LOGW(config->TAG, "Pin mode is %d", local_mode);
        }
        button_last_state = button_level;
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void button_start_task(button_config_t *config)
{
    xTaskCreate(button_task, "btn", 2048, config, 5, NULL);
}