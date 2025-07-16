#include "app_ctx.h"
#include "led_controller.h"
#include "button_controller.h"
#include "usb_transport.h"
#include "logger.h"


app_ctx_t ctx;

static void receive_report(const uint8_t *data, size_t len)
{
    log_info("Received OUT Report");
}

void app_ctx_init(void)
{
    ctx.led = &led_controller_if;
    ctx.led->init();

    ctx.button = &button_controller_if;
    ctx.button->init();

    ctx.transport = &usb_transport_if;
    ctx.transport->init(receive_report);
}
