#ifndef BUTTON_HANDLE_H // This is a "Header Guard" to prevent double loading
#define BUTTON_HANDLE_H

typedef struct
{
    int button_pin;
    int *mode;
    char *TAG;
} button_config_t;

void button_task(void *pv);
void button_start_task(button_config_t *config);

#endif