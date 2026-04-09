#ifndef LED_HANDLE_H // This is a "Header Guard" to prevent double loading
#define LED_HANDLE_H

typedef struct
{
    int led_pin;
    int *mode;
    char *tag;
} led_config_t;


void led_task(void *pv);
void led_start_task(led_config_t *config);
#endif