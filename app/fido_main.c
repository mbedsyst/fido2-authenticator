#include <zephyr/kernel.h>

#include "app_ctx.h"
#include "logger.h"

void fido_main(void)
{
    uint8_t msg[] = "Hello from nRF52";

    log_info("FIDO2 App Init");

    ctx.led->set(LED_OPERATION, LED_BLINK_SLOW);

    
    while(1)
    {
        ctx.transport->send(msg, sizeof(msg));
        k_msleep(5000);
    }
}
