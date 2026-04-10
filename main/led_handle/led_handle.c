#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <driver/ledc.h>

#include <led_handle/led_handle.h>

void led_task(void *pv)
{
    led_config_t *config = (led_config_t *)pv;
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = config->led_pin,
        .duty = 0,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel);
    int current_mode = 0;
    while (1)
    {
        if (xQueueReceive(config->mode_queue, &current_mode, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGW(config->tag, "New mode received: %d", current_mode);
            int duty = current_mode;

            // Safety check: ensure it doesn't exceed our 13-bit limit (8191)
            if (duty > 8191)
                duty = 8191;
            if (duty < 0)
                duty = 0;

            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        }
    }
}
void led_start_task(led_config_t *config)
{
    xTaskCreate(led_task, "led", 2048, config, 5, NULL);
}