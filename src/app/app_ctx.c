#include <zephyr/logging/log.h>

#include "app_ctx.h"
#include "led_controller.h"
#include "button_controller.h"
#include "usb_transport.h"

LOG_MODULE_REGISTER(app_ctx);

app_ctx_t ctx;

void app_receive_report(const uint8_t *data, size_t len)
{
    LOG_INF("Received OUT Report");
    ctaphid_receive_packet(&ctx, data, len);
}

void app_ctx_init(void)
{
    LOG_INF("Setting Application Context");

    memset(&ctx, 0, sizeof(ctx));

    ctx.led = &led_controller_if;
    ctx.led->init();

    ctx.button = &button_controller_if;
    ctx.button->init();

    ctx.transport = &usb_transport_if;
    ctx.transport->init(app_receive_report);

    LOG_INF("Application Context Initialized");
}
