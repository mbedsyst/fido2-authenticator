#include <stdio.h>

#include <zephyr/kernel.h>

#include "app_events.h"
#include "app_ctx.h"
#include "state_handlers.h"

LOG_MODULE_REGISTER(state_handler);

bool IDLE_state_handler(app_ctx_t *ctx)
{
    LOG_INF("Current State: IDLE");
    ctx->device_state = STATE_IDLE;
    ctx->led->set(LED_IDLE, LED_BLINK_SLOW);
}

bool handle_state_receiving(app_ctx_t *ctx)
{
    LOG_INF("Current State: RECEIVING");
    ctx->device_state = STATE_RECEIVING;
    ctx->led->set(LED_IDLE, LED_OFF);
    ctx->led->set(LED_OPERATION, LED_BLINK_FAST);
}

bool handle_state_reconstructing(app_event_t event)
{
    
}

bool handle_state_processing(app_event_t event)
{
    
}

bool handle_state_deconstructing(app_event_t event)
{
    
}

bool handle_state_responding(app_event_t event)
{
    
}

bool handle_state_error(app_event_t event)
{
    
}