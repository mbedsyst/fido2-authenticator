/**
 * @file    ctaphid_deconstructor.c
 * @brief   Handles fragmentation of CTAPHID Message into INIT and CONT packets
 * 
 * This module is responsible for fragmenting a Response Payload into INIT and 
 * CONT Packets to form a complete Message in a CTAPHID Transaction.
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
#include "ctaphid_deconstructor.h"

LOG_MODULE_REGISTER(ctaphid_deconstructor);

uint16_t calculate_message_size(uint16_t payload_len)
{
    if(payload_len <= INIT_DATA_MAX_LEN)
    {
        return (payload_len + 7);
    }
    else if (payload_len > INIT_DATA_MAX_LEN && payload_len <= MAX_PAYLOAD_LEN)
    {
        uint8_t cont_payload_len, cont_packet_count;
        cont_payload_len = payload_len - INIT_DATA_MAX_LEN;
        if((cont_payload_len % CONT_DATA_MAX_LEN) == 0)
        {
            cont_packet_count = cont_payload_len/CONT_DATA_MAX_LEN;
            return (cont_packet_count * (5 + CONT_DATA_MAX_LEN) + (1 * PKT_SIZE_DEFAULT));
        }
        else
        {
            uint8_t last_packet_payload_len = cont_payload_len % CONT_DATA_MAX_LEN;
            cont_packet_count = (cont_payload_len/CONT_DATA_MAX_LEN);
            return (cont_packet_count * (5 + CONT_DATA_MAX_LEN) + (last_packet_payload_len + 5) + (1 * PKT_SIZE_DEFAULT));
        }
    }
}

uint8_t calculate_packet_count(uint16_t payload_len)
{
    if(payload_len <= INIT_DATA_MAX_LEN)
    {
        return 1;
    }
    else if (payload_len > INIT_DATA_MAX_LEN && payload_len <= MAX_PAYLOAD_LEN)
    {
        uint8_t count, seq_count;
        count = payload_len - INIT_DATA_MAX_LEN;
        if((count % CONT_DATA_MAX_LEN) == 0)
        {
            seq_count = count/CONT_DATA_MAX_LEN;
            return seq_count;
        }
        else
        {
            seq_count = (count/CONT_DATA_MAX_LEN) + 1;
            return seq_count;
        }
    }
}

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

ctaphid_status_t ctaphid_payload_deconstructor(app_ctx_t *ctx)
{
    // Check if Context is Valid
    if(!ctx)
    {
        LOG_ERR("Received invalid context");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    // Check if Response Payload Length is valid
    if(ctx->response_payload_len == 0 || ctx->response_payload_len > MAX_PAYLOAD_SIZE)
    {
        LOG_ERR("Invalid Payload Length");
        return CTAPHID_ERROR_INVALID_LEN;
    }

    uint16_t payload_bytes_copied = 0;
    uint16_t payload_bytes_remaining = ctx->response_payload_len;
    uint8_t  init_packet_payload_len = 0;
    uint16_t cont_packet_payload_len = 0;
    uint16_t cont_packet_offset_in_message = 0;
    uint16_t current_cont_packet_index = 0;
    uint8_t  current_cont_packet_count = 0;

    // Calculate Total Packet Count and Response Message Size
    ctx->response_packet_count = calculate_packet_count(ctx->response_payload_len);
    ctx->response_message_len = calculate_message_size(ctx->response_payload_len);

    // Setting up the INIT Packet
    ctx->response_message[INIT_CID_POS + 0] = (ctx->active_response_cid >> 24) & 0xFF;
    ctx->response_message[INIT_CID_POS + 1] = (ctx->active_response_cid >> 16) & 0xFF;
    ctx->response_message[INIT_CID_POS + 2] = (ctx->active_response_cid >> 8) & 0xFF;
    ctx->response_message[INIT_CID_POS + 3] = (ctx->active_response_cid) & 0xFF;
    ctx->response_message[INIT_CMD_POS] = ctx->response_cmd;
    ctx->response_message[INIT_BCNTH_POS] = (ctx->response_payload_len >> 8) & 0xFF;
    ctx->response_message[INIT_BCNTL_POS] = (ctx->response_payload_len) & 0xFF;
    init_packet_payload_len = (ctx->response_payload_len < INIT_DATA_MAX_LEN) ? ctx->response_payload_len : INIT_DATA_MAX_LEN;
    memcpy(&ctx->response_message[INIT_DATA_POS], ctx->response_payload, init_packet_payload_len);
    
    // Update tracker variables
    payload_bytes_copied += init_packet_payload_len;
    payload_bytes_remaining -= init_packet_payload_len;

    // Setting up the CONT Packets
    while(payload_bytes_remaining > 0)
    {
        cont_packet_offset_in_message = PKT_SIZE_DEFAULT + (current_cont_packet_count * PKT_SIZE_DEFAULT);
        ctx->response_message[cont_packet_offset_in_message + CONT_CID_POS + 0] = (ctx->active_response_cid >> 24) & 0xFF;
        ctx->response_message[cont_packet_offset_in_message + CONT_CID_POS + 1] = (ctx->active_response_cid >> 16) & 0xFF;
        ctx->response_message[cont_packet_offset_in_message + CONT_CID_POS + 2] = (ctx->active_response_cid >> 8) & 0xFF;
        ctx->response_message[cont_packet_offset_in_message + CONT_CID_POS + 3] = (ctx->active_response_cid) & 0xFF;
        ctx->response_message[cont_packet_offset_in_message + CONT_SEQ_POS] = current_cont_packet_count;
        cont_packet_payload_len = (payload_bytes_remaining < CONT_DATA_MAX_LEN) ? payload_bytes_remaining : CONT_DATA_MAX_LEN;
        memcpy(&ctx->request_message[cont_packet_offset_in_message + CONT_DATA_POS], &ctx->response_payload[payload_bytes_copied], cont_packet_payload_len);
        
        // Update tracker variables
        payload_bytes_copied += cont_packet_payload_len;
        payload_bytes_remaining -= cont_packet_payload_len;
        current_cont_packet_count++;
    }

    // Check if Response Payload was completely Deconstructed
    if(!(payload_bytes_remaining == 0 && payload_bytes_copied = ctx->response_payload_len))
    {
        LOG_ERR("Response Payload Deconstruction failed");
        return CTAPHID_DECONSTRUCTION_FAILED;
    }

    // Check if Response Message is Sequenced correctly
    int ret = packet_order_check(ctx->response_message, ctx->response_packet_count);
    if(ret)
    {
        LOG_ERR("Payload Deconstruction failed.");
        return CTAPHID_DECONSTRUCTION_FAILED;
    }

    LOG_DBG("Response Payload Deconstructed");
    return CTAPHID_OK;

}