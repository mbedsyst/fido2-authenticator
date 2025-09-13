/**
 * @file    ctaphid_reconstructor.c
 * @brief   Handles Reconstruction of CTAPHID Payload from Request Message Buffer
 * 
 * This module is responsible for parsing the Request Message Packets
 * to form the complete Request Payload in a CTAPHID Transaction.
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

/*
    SEQ field position in CONT Packets
        4 | 1 | 1 | 1 | 57
        4 | 1 | 59          - 68
        4 | 1 | 59          - 132
        4 | 1 | 59          - 196
        4 | 1 | 59          - 260
        4 | 1 | 59          - 324
        4 | 1 | 59          - 388

    If Total Packet Count (INIT + CONT) = 96 (1 -> 96)
    SEQ Value varies from 0 -> 95
*/

static uint8_t packet_order_check(uint8_t *message, uint8_t total_packet_count)
{
    if(total_packet_count == 0 || total_packet_count > 128)
    {
        LOG_ERR("Message Buffer Order Check failed. Invalid Packet Count.");
        return 255;
    }

    uint8_t current_seq_packet_count;
    uint8_t current_seq_field_index;

    for(current_seq_packet_count = 0; current_seq_packet_count < total_packet_count; current_seq_packet_count++)
    {
        current_seq_field_index = (PKT_SIZE_DEFAULT + CONT_SEQ_POS) + (current_seq_packet_count * PKT_SIZE_DEFAULT);
        if(message[current_seq_field_index] != (current_seq_packet_count))
        {
            LOG_ERR("Message Buffer Order Check failed. SEQ Value mismatch at packet %d", current_seq_packet_count);
            return current_seq_packet_count;
        }
    }

    LOG_DBG("Message Buffer Order Check passed");
    return 0;
}

ctaphid_status_t ctaphid_payload_reconstructor(app_ctx_t *ctx)
{
    // Check if Context is Valid
    if(!ctx)
    {
        LOG_ERR("Received invalid context");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    // Check if Request Payload Length is valid
    if(ctx->request_payload_len == 0 || ctx->request_payload_len > MAX_PAYLOAD_SIZE)
    {
        LOG_ERR("Invalid Payload Length");
        return CTAPHID_ERROR_INVALID_LEN;
    }

    // Check if Request Message is Sequenced correctly
    int ret = packet_order_check(ctx->request_message, ctx->request_packet_count);
    if(ret)
    {
        LOG_ERR("Payload Reconstruction failed.");
        return CTAPHID_ERROR_INVALID_LEN;
    }

    uint16_t payload_bytes_copied = 0;

    // Calculate Size of Payload to copy for INIT Packet
    uint8_t init_packet_payload_len = (ctx->request_payload_len < INIT_DATA_MAX_LEN) ? ctx->request_payload_len : INIT_DATA_MAX_LEN;
    memcpy(ctx->request_payload, &ctx->request_message[INIT_DATA_POS], init_packet_payload_len);
    payload_bytes_copied += init_packet_payload_len;

    // Calculate Size of Remaining Payload in CONT Packets
    uint16_t payload_bytes_remaining = ctx->request_payload_len - payload_bytes_copied;
    uint8_t current_cont_packet_index = 0;

    while (payload_bytes_remaining > 0) 
    {
        // Calculate the offset in Request Message buffer to copy from
        uint16_t cont_packet_offset_in_message = PKT_SIZE_DEFAULT + current_cont_packet_index * PKT_SIZE_DEFAULT;
        // Calculate the size of payload to copy from Message Buffer
        uint16_t cont_packet_payload_len = (payload_bytes_remaining < CONT_DATA_MAX_LEN) ? payload_bytes_remaining : CONT_DATA_MAX_LEN;
        // Copy the CONT Packet Payload from Message Buffer into Payload Buffer
        memcpy(ctx->request_payload + payload_bytes_copied, &ctx->request_message[cont_packet_offset_in_message + CONT_DATA_POS], cont_packet_payload_len);
        payload_bytes_copied += cont_packet_payload_len;
        payload_bytes_remaining -= cont_packet_payload_len;
        current_cont_packet_index++;
    }

    if(!(payload_bytes_remaining == 0 && payload_bytes_copied == ctx->request_payload_len))
    {        
        LOG_ERR("Payload Reconstruction Failed.");
        return CTAPHID_ERROR_INVALID_LEN;
    }

    LOG_DBG("Request Payload reconstructed.");
    event_queue_push(EVENT_PAYLOAD_RECONSTRUCTED);

}