/**
 * @file    ctaphid_reconstructor.c
 * @brief   Handles reassembly of CTAP2 Payload from INIT and CONT packets
 * 
 * This module is responsible for assembling INIT and CONT Packets
 * to form a complete Message in a CTAPHID Transaction
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
#include "ctaphid_reconstructor.h"


LOG_MODULE_REGISTER(ctaphid_reconstructor);

ctaphid_status_t ctaphid_payload_reconstructor(app_ctx_t *ctx)
{
    // Check if Context is Valid
    if(!ctx)
    {
        LOG_ERR("Received invalid context");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    if(ctx->request_payload_len == 0 || ctx->request_payload_len > MAX_PAYLOAD_SIZE)
    {
        LOG_ERR("Invalid Payload Length");
        return CTAPHID_ERROR_INVALID_LEN;
    }

    uint16_t data_copied = 0;

    // Calculate Size of Payload to copy for INIT Packet
    uint8_t init_payload_len = (ctx->request_payload_len < INIT_DATA_MAX_LEN) ? ctx->request_payload_len : INIT_DATA_MAX_LEN;
    memcpy(ctx->request_payload, &ctx->request_message[INIT_DATA_POS], init_payload_len);
    data_copied += init_payload_len;

    // Calculate Size of Remaining Payload in CONT Packets
    uint16_t data_remaining = ctx->request_payload_len - data_copied;
    uint16_t cont_packet_index = 0;

    while (data_remaining > 0) 
    {
        // Calculate the offset in Request Message buffer to copy from
        uint16_t cont_offset = PKT_SIZE_DEFAULT + cont_packet_index * PKT_SIZE_DEFAULT;
        // Calculate the size of payload to copy from Message Buffer
        uint16_t copy_len = (data_remaining < CONT_DATA_MAX_LEN) ? data_remaining : CONT_DATA_MAX_LEN;
        // Copy the CONT Packet Payload from Message Buffer into Payload Buffer
        memcpy(ctx->request_payload + data_copied, &ctx->request_message[cont_offset + 5], copy_len);
        data_copied += copy_len;
        data_remaining -= copy_len;
        cont_packet_index++;
    }

    LOG_DBG("Request Payload reconstructed.");
    event_queue_push(EVENT_PAYLOAD_RECONSTRUCTED);
}