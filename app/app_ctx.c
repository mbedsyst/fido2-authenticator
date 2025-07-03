#include "app_ctx.h"
#include "led_controller.h"

app_ctx_t ctx;

void app_ctx_init(void)
{
    ctx.led = &led_controller_if;
    ctx.led->init();
}