#include <stdio.h>

#include <zephyr/kernel.h>

#include "app_events.h"
#include "app_ctx.h"
#include "ctaphid_reconstructor.h"
#include "ctaphid_deconstructor.h"
#include "event_queue.h"
#include "state_handlers.h"

LOG_MODULE_REGISTER(state_handler);

bool IDLE_state_handler(app_ctx_t *ctx)
{
    LOG_INF("Current State: IDLE");
    ctx->device_state = STATE_IDLE;
    ctx->led->set(LED_STATUS, LED_OFF);
    ctx->led->set(LED_OPERATION, LED_OFF);
    ctx->led->set(LED_ERROR, LED_OFF);
    ctx->led->set(LED_IDLE, LED_BLINK_SLOW);
}

bool RECEIVING_state_handler(app_ctx_t *ctx)
{
    LOG_INF("Current State: RECEIVING");
    ctx->device_state = STATE_RECEIVING;
    ctx->led->set(LED_ERROR, LED_OFF);
    ctx->led->set(LED_OPERATION, LED_OFF);
    ctx->led->set(LED_IDLE, LED_OFF);
    ctx->led->set(LED_STATUS, LED_BLINK_FAST);
}

bool RECONSTRUCTING_state_handler(app_ctx_t *ctx)
{
    LOG_INF("Current State: RECONSTRUCTING");
    ctx->device_state = STATE_RECONSTRUCTING;
    ctx->led->set(LED_STATUS, LED_OFF);
    ctx->led->set(LED_ERROR, LED_OFF);
    ctx->led->set(LED_IDLE, LED_OFF);
    ctx->led->set(LED_OPERATION, LED_ON);

    int ret = 0;

    ret = ctaphid_payload_reconstructor(ctx);
    if(ret > 0)
    {
        LOG_ERR("Payload Reconstructor failed with Error Code: %d", ret);
        // ToDo: Match Return Error Code to CTAPHID Specification
        ctx->error_code = ret;
        event_queue_push(EVENT_ERROR_OCCURED);        
    }
}

bool handle_state_processing(app_event_t event)
{
    LOG_INF("Current State: PROCESSING");
    ctx->device_state = STATE_PROCESSING;
    ctx->led->set(LED_STATUS, LED_OFF);
    ctx->led->set(LED_ERROR, LED_OFF);
    ctx->led->set(LED_IDLE, LED_OFF);
    ctx->led->set(LED_OPERATION, LED_ON);

    int ret = 0;

    // Call the CTAPHID Command Handler.
    if(ret > 0)
    {
        LOG_ERR("Payload Reconstructor failed with Error Code: %d", ret);
        // ToDo: Match Return Error Code to CTAPHID Specification
        ctx->error_code = ret;
        event_queue_push(EVENT_ERROR_OCCURED);        
    }

    // Call the CTAP2 Parser and Handler.
    if(ret > 0)
    {
        LOG_ERR("Payload Reconstructor failed with Error Code: %d", ret);
        // ToDo: Match Return Error Code to CTAPHID Specification
        ctx->error_code = ret;
        event_queue_push(EVENT_ERROR_OCCURED);        
    }
}

bool DECONSTRUCTING_state_handler(app_ctx_t *ctx)
{
    LOG_INF("Current State: DECONSTRUCTING");
    ctx->device_state = STATE_DECONSTRUCTING;
    ctx->led->set(LED_STATUS, LED_OFF);
    ctx->led->set(LED_ERROR, LED_OFF);
    ctx->led->set(LED_IDLE, LED_OFF);
    ctx->led->set(LED_OPERATION, LED_ON);

    int ret = 0;

    ret = ctaphid_payload_deconstructor(ctx);
    if(ret > 0)
    {
        LOG_ERR("Payload Deconstructor failed with Error Code: %d", ret);
        // ToDo: Match Return Error Code to CTAPHID Specification
        ctx->error_code = ret;
        event_queue_push(EVENT_ERROR_OCCURED);        
    }
}

bool RESPONDING_state_handler(app_ctx_t *ctx)
{
    LOG_INF("Current State: RESPONDING");
    ctx->device_state = STATE_RESPONDING;
    ctx->led->set(LED_ERROR, LED_OFF);
    ctx->led->set(LED_OPERATION, LED_OFF);
    ctx->led->set(LED_IDLE, LED_OFF);
    ctx->led->set(LED_STATUS, LED_BLINK_FAST);

    int ret = 0;
    int current_packet_count;

    for(current_packet_count = 0; current_packet_count < ctx->response_packet_count; current_packet_count++)
    {
        // ToDo: Fix the buffer positioning and size of bytes to send over
        ret = ctx->transport->send(ctx->response_message[current_packet_count * 64], 64);
        if(ret > 0)
        {
            LOG_ERR("Response failed with Error Code: %d", ret);
            // ToDo: Match Return Error Code to CTAPHID Specification
            ctx->error_code = ret;
            event_queue_push(EVENT_ERROR_OCCURED);     
        }
    }
}

bool handle_state_error(app_event_t event)
{
    LOG_INF("Current State: ERROR");
    ctx->device_state = STATE_ERROR;
    ctx->led->set(LED_STATUS, LED_OFF);
    ctx->led->set(LED_OPERATION, LED_OFF);
    ctx->led->set(LED_IDLE, LED_OFF);
    ctx->led->set(LED_ERROR, LED_BLINK_SLOW);
}