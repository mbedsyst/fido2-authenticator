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

    ctx.led = &led_controller_if;
    ctx.led->init();

    ctx.button = &button_controller_if;
    ctx.button->init();

    ctx.transport = &usb_transport_if;
    ctx.transport->init(app_receive_report);

    // Clear all buffers and metadata
    memset(ctx.request_message, 0, sizeof(ctx.request_message));
    memset(ctx.request_payload, 0, sizeof(ctx.request_payload));
    memset(ctx.response_payload, 0, sizeof(ctx.response_payload));
    memset(ctx.response_message, 0, sizeof(ctx.response_message));

    ctx.device_state = STATE_IDLE;
    ctx.initialized = false;

    ctx.request_channel_id = 0;
    ctx.request_cmd = 0;
    ctx.request_message_len = 0;
    ctx.request_payload_len = 0;
    ctx.response_payload_len = 0;
    ctx.response_message_len = 0;

    LOG_INF("Application Context Initialized");
}
