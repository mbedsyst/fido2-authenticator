/**
 * @file    ctaphid_deconstructor.c
 * @brief   Handles fragmentation of CTAPHID Message into INIT and CONT packets
 * 
 * This module is responsible for fragmenting into INIT and 
 * CONT Packets to form a complete Message in a CTAPHID Transaction
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

ctaphid_payload_deconstructor(app_ctx_t *ctx)
{
    size_t total_packets = 0;
    size_t offset = 0;

    /*** Setting up the INIT Packet ***/
    // Setting Channel ID in the INIT Packet
    ctx->response_message[CID_POS + 0] = (ctx->request_channel_id >> 24) & 0xFF;
    ctx->response_message[CID_POS + 1] = (ctx->request_channel_id >> 16) & 0xFF;
    ctx->response_message[CID_POS + 2] = (ctx->request_channel_id >> 8) & 0xFF;
    ctx->response_message[CID_POS + 3] = (ctx->request_channel_id) & 0xFF;
    // Setting CMD, BCNTH and BCNTL in the INIT Packet
    ctx->response_message[4] = 0x80 | (ctx->request_cmd & 0x7F);            // Ensure MSB is set for INIT CMD
    ctx->response_message[5] = (ctx->response_payload_len >> 8) & 0xFF;     // BCNTH
    ctx->response_message[6] = (ctx->response_payload_len) & 0xFF;          // BCNTL
    // Copying valid data for INIT Packet from Response Payload buffer
    uint8_t init_data_len = (ctx->response_payload_len > INIT_DATA_MAX_LEN) ? INIT_DATA_MAX_LEN : ctx->response_payload_len;
    memcpy(&ctx->response_message[INIT_DATA_POS], ctx->response_payload, init_data_len);
    offset += PKT_SIZE_DEFAULT;
    total_packets++;

    size_t remaining = ctx->response_payload_len - init_data_len;
    size_t payload_offset = init_data_len;

    // Setting up the CONT Packets
    uint8_t seq = 0;

    while (remaining > 0) 
    {
        uint8_t* cont_pkt = out_buf + offset;

        cont_pkt[0] = (cid >> 24) & 0xFF;
        cont_pkt[1] = (cid >> 16) & 0xFF;
        cont_pkt[2] = (cid >> 8)  & 0xFF;
        cont_pkt[3] = (cid)       & 0xFF;

        cont_pkt[4] = seq++;

        size_t chunk = (remaining > CONT_DATA_MAX_LEN) ? CONT_DATA_MAX_LEN : remaining;
        memcpy(&cont_pkt[5], &payload[payload_offset], chunk);

        payload_offset += chunk;
        remaining -= chunk;
        offset += PKT_SIZE_DEFAULT;
        total_packets++;
    }

    LOG_DBG("Deconstructed Response Payload into Message Buffer.");
    event_queue_push(EVENT_PAYLOAD_DECONSTRUCTED);
}