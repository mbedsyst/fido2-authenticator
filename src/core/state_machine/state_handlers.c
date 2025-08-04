#include <stdio.h>

#include <zephyr/kernel.h>

#include "app_events.h"
#include "app_ctx.h"
#include "ctaphid_reconstructor.h"
#include "ctaphid_deconstructor.h"
#include "event_queue.h"
#include "state_handlers.h"
#include "error_mapper.h"

LOG_MODULE_REGISTER(state_handler);

static void transition_to(app_state_t new_state)
{
    current_state = new_state;
    event_queue_push(EVENT_NONE);
}

void handle_idle(app_ctx_t *ctx, app_event_t event)
{
    switch(event)
    {
        case EVENT_NONE:
            LOG_INF("Current State: IDLE");
            ctx->device_state = STATE_IDLE;
            ctx->led->set(LED_STATUS, LED_OFF);
            ctx->led->set(LED_OPERATION, LED_OFF);
            ctx->led->set(LED_ERROR, LED_OFF);
            ctx->led->set(LED_IDLE, LED_BLINK_SLOW);
            break;

        case EVENT_REQUEST_RECEIVING_STARTED:
            LOG_INF("Transitioning to State: RECEIVING");
            transition_to(STATE_RECEIVING_STARTED);
            break;

        case EVENT_ERROR_OCCURED:
            LOG_INF("Transitioning to State: ERROR");
            transition_to(STATE_ERROR);
            break;

        default:
            LOG_WRN("Event Unrecognized in IDLE state");
            break;
    }
}

void handle_receiving(app_ctx_t *ctx, app_event_t event)
{
    switch(event)
    {
        case EVENT_NONE:
            LOG_INF("Current State: RECEIVING");
            ctx->device_state = STATE_RECEIVING_STARTED;
            ctx->led->set(LED_ERROR, LED_OFF);
            ctx->led->set(LED_OPERATION, LED_OFF);
            ctx->led->set(LED_IDLE, LED_OFF);
            ctx->led->set(LED_STATUS, LED_BLINK_FAST);
            break;

        case EVENT_REQUEST_RECEIVING_FINISHED:
            LOG_INF("Transitioning to State: RECONSTRUCTING");
            transition_to(STATE_RECONSTRUCTING);
            break;
        
        case EVENT_ERROR_OCCURED:
            LOG_INF("Transitioning to State: ERROR");
            transition_to(STATE_ERROR);
            break;

        default:
            LOG_WRN("Event Unrecognized in RECEIVING state");
            break;
    }

}

void handle_reconstructing(app_ctx_t *ctx, app_event_t event)
{
    switch(event)
    {
        case EVENT_NONE:
            LOG_INF("Current State: RECONSTRUCTING");
            ctx->led->set(LED_STATUS, LED_OFF);
            ctx->led->set(LED_ERROR, LED_OFF);
            ctx->led->set(LED_IDLE, LED_OFF);
            ctx->led->set(LED_OPERATION, LED_ON);

            if (ctaphid_payload_reconstructor(ctx) > 0)
            {
                LOG_ERR("Payload Reconstructor failed with Error Code: %d", ctx->error_code);
                event_queue_push(EVENT_ERROR_OCCURED);
            }
            else
            {
                event_queue_push(EVENT_PROCESSING_DONE);
            }
            break;

        case EVENT_PAYLOAD_RECONSTRUCTED:
            LOG_INF("Transitioning to State: PROCESSING");
            transition_to(STATE_PROCESSING);
            break;

        case EVENT_ERROR_OCCURED:
            LOG_INF("Transitioning to State: ERROR");
            transition_to(STATE_ERROR);
            break;

        default:
            LOG_WRN("Event Unrecognized in IDLE state");
            break;

    }
}

void handle_processing(app_ctx_t *ctx, app_event_t event)
{
    switch(event)
    {
        case EVENT_NONE:
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
            break;

        case EVENT_PROCESSING_DONE:
            LOG_INF("Transitioning to State: DECONSTRUCTING");
            transition_to(STATE_DECONSTRUCTING);
            break;

        case EVENT_ERROR_OCCURED:
            LOG_INF("Transitioning to State: ERROR");
            transition_to(STATE_ERROR);
            break;

        default:
            LOG_WRN("Unrecognized Event for PROCESSING State");
            break;

    }
}

void handle_deconstructing(app_ctx_t *ctx, app_event_t event)
{
    switch(event)
    {
        case EVENT_NONE:
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
            break;

        case EVENT_PAYLOAD_DECONSTRUCTED:
            LOG_INF("Transitioning to State: RESPONDING");
            transition_to(STATE_RESPONDING_STARTED);
            break;

        case EVENT_ERROR_OCCURED:
            LOG_INF("Transitioning to State: ERROR");
            transition_to(STATE_ERROR);
            break;

        default:
            LOG_WRN("Event Unrecognized in DECONSTRUCTING state");
            break;
    }
}

void handle_responding(app_ctx_t *ctx, app_event_t event)
{
    switch(event)
    {
        case EVENT_NONE:
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
            break;

        case EVENT_RESPONSE_TRANSMITTED:
            LOG_INF("Transitioning to State: IDLE");
            transition_to(STATE_IDLE);
            break;

        case EVENT_ERROR_OCCURED:
            LOG_INF("Transitioning to State: ERROR");
            transition_to(STATE_ERROR);
            break;

        default:
            LOG_WRN("Event Unrecognized in RESPONDING state");
            break;
    }
}

void handle_error(app_ctx_t *ctx, app_event_t event)
{
    switch(event)
    {
        case EVENT_NONE:
            LOG_INF("Current State: ERROR");
            ctx->device_state = STATE_ERROR;
            ctx->led->set(LED_STATUS, LED_OFF);
            ctx->led->set(LED_OPERATION, LED_OFF);
            ctx->led->set(LED_IDLE, LED_OFF);
            ctx->led->set(LED_ERROR, LED_BLINK_SLOW);
            remap_error_to_ctaphid(ctx);
            // Write a function to prepare message buffer
            // Write a function to send out the message packet. Raise EVENT_ERROR_HANDLED
            break;

        case EVENT_ERROR_HANDLED:
            LOG_INF("Transitioning to State: IDLE");
            transition_to(STATE_IDLE);

        default:
            LOG_WRN("Event Unrecognized in ERROR state");
            break;
    }
}