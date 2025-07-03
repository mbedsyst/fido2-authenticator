#include <zephyr/kernel.h>

#include "app_ctx.h"
#include "logger.h"

static void on_long_press(void *user)
{
    printk("Long Press Detected!\n");
    ctx.led->set(LED_STATUS, LED_BLINK_FAST);  // Visual cue
}

void fido_main(void)
{
    log_info("FIDO2 App Init\n");

    ctx.button->register_long_press_cb(on_long_press, NULL);

    while(1)
    {

    }
}
