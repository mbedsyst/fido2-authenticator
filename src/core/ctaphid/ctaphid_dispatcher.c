/**
 * @file    ctaphid_dispatcher.c
 * @brief   Handles command dispatching for CTAPHID Commands
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
#include "ctaphid_dispatcher.h"

LOG_MODULE_REGISTER(ctaphid_dispatcher);

ctaphid_status_t ctaphid_cmd_msg(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    // Setup the Error Response
    ctx->request_cmd = CTAPHID_ERROR;
    ctx->response_payload_len = 1;
    ctx->response_payload[0] = ERR_INVALID_CMD;
    event_queue_push(EVENT_PROCESSING_DONE);

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

ctaphid_status_t ctaphid_cmd_cancel(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the CANCEL Command Logic
}

ctaphid_status_t ctaphid_cmd_error(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the ERROR Command Logic
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

