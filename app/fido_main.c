#include <zephyr/kernel.h>

#include "app_ctx.h"
#include "logger.h"

void fido_main(void)
{
    printk("FIDO2 App Init\n");

    log_debug("Blinking STATUS LED at 1Hz");
    ctx.led->set(LED_STATUS, LED_BLINK_SLOW);

    log_info("Blinking ERROR LED at 5Hz");
    ctx.led->set(LED_ERROR, LED_BLINK_FAST);
    
    log_warn("Switching ON IDLE LED");
    ctx.led->set(LED_IDLE, LED_ON);
    
    log_error("Switching ON OPERATION LED");
    ctx.led->set(LED_OPERATION, LED_ON);

    while(1)
    {

    }
}
