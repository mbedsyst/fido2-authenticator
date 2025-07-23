/**
 * @file    ctaphid_handler.c
 * @brief   Handlers for different CTAPHID Commands
 * 
 * This module is responsible for calling the respective Command
 * Dispatcher based on the CTAPHID Command
 * 
 * @author  Zalman Ul Fariz
 * @date    July 2025
 */

#include <stddef.h>
#include <stdint.h>

#include <zephyr/logging/log.h>

#include "app_ctx.h"
#include "event_queue.h"
#include "ctaphid.h"
#include "ctaphid_handler.h"

LOG_MODULE_REGISTER(ctaphid_dispatcher);

ctaphid_status_t ctaphid_cmd_msg(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    // Setup the Error Response
    LOG_ERR("Device does not support U2F Processing");
    ctaphid_cmd_error(ctx, ERR_INVALID_CMD);
    return CTAPHID_OK;
}

ctaphid_status_t ctaphid_cmd_cbor(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the CBOR Command Logic
}

ctaphid_status_t ctaphid_cmd_init(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the INIT Command Logic
}

ctaphid_status_t ctaphid_cmd_ping(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    // Setup the Ping Response
    ctx->response_payload_len = ctx->request_payload_len;
    memcpy(ctx->response_payload, ctx->request_payload, ctx->response_payload_len);
    
    event_queue_push(EVENT_PROCESSING_DONE);

    return CTAPHID_OK;
}

void ctaphid_cmd_cancel(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    /**
     * 
     */
}

void ctaphid_cmd_error(app_ctx_t *ctx, uint8_t error_code)
{
    if(error_code == 0)
    {
        LOG_ERR("Received Invalid Error Code");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // Setup Response Frame
    ctx->request_cmd = CTAPHID_ERROR;
    ctx->response_payload_len = 1;
    ctx->response_payload = error_code;

    event_queue_push(EVENT_PROCESSING_DONE);
}

ctaphid_status_t ctaphid_cmd_keepalive()
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the KEEPALIVE Command Logic
}

