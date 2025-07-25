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
    LOG_ERR("Device does not support U2F Processing");
    return CTAPHID_ERROR_INVALID_CMD;
}

ctaphid_status_t ctaphid_cmd_cbor(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the CBOR Command Logic
}

ctaphid_status_t ctaphid_cmd_init(app_ctx_t *ctx)
{



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

ctaphid_status_t ctaphid_cmd_cancel(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
}

ctaphid_status_t ctaphid_cmd_error(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // Setup Response Frame
    ctx->request_cmd = CTAPHID_ERROR;
    ctx->response_payload_len = 1;
    ctx->response_payload = error_code;

    event_queue_push(EVENT_PROCESSING_DONE);
}

ctaphid_status_t ctaphid_cmd_keepalive(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the KEEPALIVE Command Logic
}

ctaphid_status_t ctaphid_cmd_dispatcher(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    switch(ctx->request_cmd)
    {
        case CTAPHID_MSG:
            LOG_DBG("Requested CTAPHID_MSG Command");
            return ctaphid_cmd_msg(ctx);
            break;

        case CTAPHID_CBOR:
            LOG_DBG("Requested CTAPHID_CBOR Command");
            return ctaphid_cmd_cbor(ctx);
            break;

        case CTAPHID_INIT:
            LOG_DBG("Requested CTAPHID_INIT Command");
            return ctaphid_cmd_init(ctx);
            break;

        case CTAPHID_PING:
            LOG_DBG("Requested CTAPHID_PING Command");
            return ctaphid_cmd_ping(ctx);
            break;

        case CTAPHID_CANCEL:
            LOG_DBG("Requested CTAPHID_CANCEL Command");
            return ctaphid_cmd_cancel(ctx);
            break;

        case CTAPHID_ERROR:
            LOG_DBG("Requested CTAPHID_ERROR Command");
            return ctaphid_cmd_error(ctx);
            break;

        case CTAPHID_KEEPALIVE:
            LOG_DBG("Requested CTAPHID_KEEPALIVE Command");
            return ctaphid_cmd_keepalive(ctx);
            break;

        default:
            LOG_DBG("Requested Command does not match any valid values");
            return CTAPHID_ERROR_INVALID_CMD;
            break;
    }
}
