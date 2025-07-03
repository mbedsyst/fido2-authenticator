#ifndef APP_APP_CTX_H_
#define APP_APP_CTX_H_

#include "led_if.h"

typedef struct
{
    const led_if_t *led;
} app_ctx_t;

extern app_ctx_t ctx;

void app_ctx_init(void);

#endif