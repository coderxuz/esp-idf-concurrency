#include <esp_http_server.h>
#include "freertos/queue.h"
#include "esp_log.h"

// We need access to the same queue your button uses
static QueueHandle_t web_queue;

esp_err_t mode_get_handler(httpd_req_t *req)
{
    char *buf;
    size_t buf_len;

    // 1. Get the length of the query string (the part after '?')
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        // 2. Actually copy the query string into our buffer
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK)
        {
            ESP_LOGI("SERVER", "Found URL query => %s", buf);
            char param[32];
            // 3. Now we can look for "val" inside that query buffer
            if (httpd_query_key_value(buf, "val", param, sizeof(param)) == ESP_OK)
            {
                ESP_LOGI("SERVER", "Found val => %s", param);
                int new_mode = atoi(param);

                // Send to the same queue your button uses!
                xQueueSend(web_queue, &new_mode, 0);
            }
        }
        free(buf); // Don't forget to free the memory!
    }

    const char *resp =
        "<html>"
        "<head><meta name='viewport' content='width=device-width, initial-scale=1'></head>"
        "<body>"
        "   <h1>ESP32 PWM Dimmer</h1>"
        "   <input type='range' min='0' max='8191' value='1000' "
        "          style='width:80%' oninput='updateMode(this.value)'>"
        "   <p>Brightness: <span id='valDisplay'>1000</span></p>"

        "   <script>"
        "       function updateMode(val) {"
        "           document.getElementById('valDisplay').innerHTML = val;"
        "           fetch('/mode?val=' + val);" // Sends the command in the background
        "       }"
        "   </script>"
        "</body>"
        "</html>";

    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Configuration for the URI
httpd_uri_t uri_mode = {
    .uri = "/mode",
    .method = HTTP_GET,
    .handler = mode_get_handler,
    .user_ctx = NULL};

// Function to start the server
void start_webserver(QueueHandle_t shared_queue)
{
    web_queue = shared_queue; // Save the queue handle
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_mode);
        ESP_LOGI("SERVER", "Web server started on port 80");
    }
}