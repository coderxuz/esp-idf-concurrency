#ifndef SERVER_HANDLE_H
#define SERVER_HANDLE_H
#include "freertos/queue.h"

void start_webserver(QueueHandle_t shared_queue);

#endif