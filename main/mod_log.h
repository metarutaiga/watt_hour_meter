/* ESProom

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#ifndef _MOD_LOG_H_
#define _MOD_LOG_H_

#include <esp_http_server.h>

extern unsigned char LOG_BUFFER[8][128];
extern unsigned char LOG_INDEX;

void mod_log(void);

void mod_log_http_handler(httpd_req_t *req);

#endif
