/* ESP HTTP Client Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_http_server.h>

#include "watt_hour_meter.h"
#include "web_server.h"

static char HTML[16384];

static const char *TAG = "WEB-SERVER";

/* An HTTP GET handler */
esp_err_t home_get_handler(httpd_req_t *req)
{
    char *html = HTML;
    time_t now = 0;
    struct tm timeinfo = { 0 };

    time(&now);
    localtime_r(&now, &timeinfo);

    // Head
    html += sprintf(html,
                    "<!DOCTYPE html>"
                    "<html>"
                    "<head>"
                    "<title>%s : Watt-Hour Meter</title>"
                    "</head>"
                    "<body>", CONFIG_AREA);

    // Area
    html += sprintf(html, "<h1>%s</h1>", CONFIG_AREA);

    // Date Time
    html += sprintf(html, "<p>%d.%d.%d %d:%d:%d</p>", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    // Table
    html += sprintf(html, "%s", "<table style=\"width:100%\">");
    html += sprintf(html, "<tr>");
    html += sprintf(html, "<th>Day</th>");
    for (int hour = 0; hour < 24; ++hour) {
        html += sprintf(html, "<th>%02d</th>", hour);
    }
    html += sprintf(html, "<th>Total</th>");
    html += sprintf(html, "</tr>");
    for (int day = 1; day < 32; ++day) {
        int total = 0;

        html += sprintf(html, "<tr>");
        html += sprintf(html, "<th>%d</th>", day);
        for (int hour = 0; hour < 24; ++hour) {
            html += sprintf(html, "<th>%d</th>", PULSE_PER_HOUR[day][hour]);
            total += PULSE_PER_HOUR[day][hour];
        }
        html += sprintf(html, "<th>%d</th>", total);
        html += sprintf(html, "</tr>");
    }
    html += sprintf(html, "</table>");

    // Tail
    html += sprintf(html,
                    "</body>"
                    "</html>");

    // Send response
    httpd_resp_send(req, HTML, strlen(HTML));

    return ESP_OK;
}

httpd_uri_t home = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = home_get_handler,
};

httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &home);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_stop(server);
}
