#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "app_ctx.h"

LOG_MODULE_REGISTER(fido_main);

void fido_main(void)
{
    uint8_t msg[] = "Hello from nRF52";

    LOG_INF("FIDO2 App Init");

    ctx.led->set(LED_OPERATION, LED_BLINK_SLOW);

    while(1)
    {
        LOG_INF("Sending Dummy Report");
        ctx.transport->send(msg, sizeof(msg));
        k_msleep(5000);
    }
}
