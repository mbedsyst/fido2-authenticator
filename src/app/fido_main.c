#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "app_ctx.h"
#include "state_machine.h"
#include "event_queue.h"
#include "app_events.h"

LOG_MODULE_REGISTER(fido_main);

void fido_main(void)
{
    LOG_INF("FIDO2 App Init");

    ctx.led->set(LED_OPERATION, LED_BLINK_SLOW);

    state_machine_init();  // Initialize state machine

    while (1)
    {
        app_event_t event;

        if (event_queue_pop(&event))  // Only process if an event exists
        {
            state_machine_handle_event(event);
        }
        else
        {
            // Optional: Sleep to avoid tight loop
            k_msleep(10);
        }
    }
}