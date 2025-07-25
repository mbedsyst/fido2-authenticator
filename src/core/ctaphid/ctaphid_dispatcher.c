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


/** @brief  Since U2F is not being supported, we have to return an Error.
 *          We should set the error to ERR_INVALID_CMD code, and then call 
 *          the CTAPHID_ERROR handler that returns the Error Code to the
 *          Client.
*/
ctaphid_status_t ctaphid_cmd_msg(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    ctx->active_response_cid = ctx->active_request_cid;
    ctx->response_cmd = ctx->request_cmd;
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
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    uint32_t new_generated_cid = 0;
    // *** Generate a new CID ***

    memcpy(ctx->response_payload, ctx->init_command_nonce, INIT_CMD_NONCE_LEN);
    ctx->response_payload[INIT_CMD_CID_POS + 0] = (new_generated_cid >> 24) & 0xFF;
    ctx->response_payload[INIT_CMD_CID_POS + 1] = (new_generated_cid >> 16) & 0xFF;
    ctx->response_payload[INIT_CMD_CID_POS + 2] = (new_generated_cid >> 8) & 0xFF;
    ctx->response_payload[INIT_CMD_CID_POS + 3] = (new_generated_cid) & 0xFF;
    ctx->response_payload[INIT_CMD_PROTOCOL_POS] = CTAPHID_PROTOCOL_VER;
    ctx->response_payload[INIT_CMD_MAJ_VER_POS] = MAJOR_DEV_VER_NUMBER;
    ctx->response_payload[INIT_CMD_MIN_VER_POS] = MINOR_DEV_VER_NUMBER;
    ctx->response_payload[INIT_CMD_BUILD_VER_POS] = BUILD_DEV_VER_NUMBER;
    ctx->response_payload[INIT_CMD_CPBLT_POS] = (CAPABILITY_WINK) | (CAPABILITY_CBOR) | (CAPABILITY_NMSG);
    ctx->response_payload_len = 17;
    
    ctx->active_response_cid = ctx->active_request_cid;
    ctx->response_cmd = ctx->request_cmd;

    event_queue_push(EVENT_PROCESSING_DONE);

    return CTAPHID_OK;
}

ctaphid_status_t ctaphid_cmd_ping(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    ctx->response_payload_len = ctx->request_payload_len;
    memcpy(ctx->response_payload, ctx->request_payload, ctx->request_payload_len);

    ctx->active_response_cid = ctx->active_request_cid;
    ctx->response_cmd = ctx->request_cmd;

    event_queue_push(EVENT_PROCESSING_DONE);

    return CTAPHID_OK;
}


/** @brief  We receive the CTAPHID_CANCEL in the midst of a transaction.
 *          When we receive that request, we set the ctx->abort_requested
 *          flag. In the currently executing function, we will keep on
 *          checking if the flag is set, and if it is, we then set the
 *          ctx->response_payload[0] to 0x2D and push the event 
 *          EVENT_PROCESSING_DONE to the queue, and then return.
 */
ctaphid_status_t ctaphid_cmd_cancel(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    ctx->abort_requested = true;
    // *** Kick off the Escape-Hatch Routine ***

    return CTAPHID_OK;
}

/** @brief We need this handler to reply to any and all operations. Once an
 *         Error event is pushed to the queue along with the local error code,
 *         we will remap that error code to a CTAPHID Specification error code.
 *         Then, we push EVENT_PROCESSING_DONE or, better EVENT_RECONSTRUCTING_DONE,
 *         and then return the control.
 */
ctaphid_status_t ctaphid_cmd_error(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    ctx->response_cmd = CTAPHID_ERROR;
    ctx->response_payload_len = 1;
    ctx->response_payload[0] = ctx->remapped_error_code;

    event_queue_push(EVENT_PROCESSING_DONE);

    return CTAPHID_OK;
}


/** @brief  This handler is called by an ongoing operation during every
 *          100ms and if the status changes between STATUS_PROCESSING and
 *          STATUS_UPNEEDED in the operation. Once it is called, we copy 
 *          the active CID to a temporary CID, and then reply with the
 *          command CTAPHID_KEEPALIVE with BCNT set to 1 and whichever is
 *          the current status code.
 */
ctaphid_status_t ctaphid_cmd_keepalive(app_ctx_t *ctx)
{
    if(!ctx)
    {
        LOG_ERR("Received Invalid Input");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    ctx->active_response_cid = ctx->active_request_cid;
    ctx->response_payload_len = 1;
    ctx->response_payload[0] = ctx->device_status_code;

    event_queue_push(EVENT_KEEPALIVE_REQUESTED);

    return CTAPHID_OK;
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
