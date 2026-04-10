#ifndef BUTTON_HANDLE_H // This is a "Header Guard" to prevent double loading
#define BUTTON_HANDLE_H

typedef struct
{
    int button_pin;
    QueueHandle_t mode_queue;
    char *TAG;
} button_config_t;

void button_task(void *pv);
void button_start_task(button_config_t *config);

#endif