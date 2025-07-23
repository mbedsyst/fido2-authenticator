/**
 * @file    ctaphid_receiver.c
 * @brief   Handles reception of CTAPHID INIT and CONT packets
 * 
 * This module is responsible for receiving the INIT and CONT Packets
 * to reconstruct the CTAPHID Message later.
 * 
 * @author  Zalman Ul Fariz
 * @date    July 2025
 */

#include <stdint.h>

#include "ctaphid.h"
#include "app_ctx.h"
#include "event_queue.h"
#include "ctaphid_receiver.h"

uint16_t CalculateMessageSize(uint16_t total_payload_len)
{
    if(total_payload_len <= INIT_DATA_MAX_LEN)
    {
        return (total_payload_len + 7);
    }
    else if (total_payload_len > INIT_DATA_MAX_LEN && total_payload_len <= PKT_MAX_PAYLOAD_LEN)
    {
        uint8_t cont_payload_len, cont_packet_count;
        cont_payload_len = total_payload_len - INIT_DATA_MAX_LEN;
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

uint8_t CalculatePacketCount(uint16_t payload_len)
{
    if(payload_len <= INIT_DATA_MAX_LEN)
    {
        return 1;
    }
    else if (payload_len > INIT_DATA_MAX_LEN && payload_len <= PKT_MAX_PAYLOAD_LEN)
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

ctaphid_status_t ctaphid_receive_packet(app_ctx_t *ctx, uint8_t *report, uint8_t len)
{
    uint8_t channel_id[CID_LEN] = {0};
    uint8_t broadcast_cid[CID_LEN] = {0xFF, 0xFF, 0xFF, 0xFF};

    static uint16_t total_len = 0;
    static uint16_t received_len = 0;
    static uint8_t total_packet_count = 0;
    static uint8_t received_packet_count = 0;

    // Retrieve the Channel ID from Report
    memcpy(channel_id, report, CID_LEN);

    // Check if Packet is an INIT Packet
    if(report[INIT_CMD_POS] & 0x80 != 0)
    {
        if(report[INIT_CMD_POS] == CTAPHID_CANCEL)
        {
            /*** Process CANCEL ***/
        }
        else if(report[INIT_CMD_POS] == CTAPHID_INIT)
        {
            /*** Copy 8-byte Nonce ***/

            if(memcmp(channel_id, broadcast_cid, CID_LEN) == 0)
            {
                /*** Process INIT NEW CID ***/
            }
            else if(memcmp(channel_id, active_CID, CID_LEN) == 0)
            {
                /*** Process INIT SAME CID ***/
            }
        }

        ctx->request_channel_id = (channel_id[0] << 24)
                                | (channel_id[1] << 16)
                                | (channel_id[2] << 8)
                                | (channel_id[3] << 0);
        ctx->request_cmd = report[INIT_CMD_POS];
        ctx->request_payload_len = ((report[INIT_BCNTH_POS]) << 8) | (report[INIT_BCNTL_POS]);
    }

    if(ctx->request_payload_len == 0 || ctx->request_payload_len > MAX_PAYLOAD_SIZE)
    {
        LOG_ERR("Invalid Payload Length");
        return CTAPHID_ERROR_INVALID_LEN;
    }

    ctx->request_message_len = CalculateMessageSize(ctx->request_payload_len);
    total_packet_count = CalculatePacketCount(ctx->request_payload_len);
    total_len = ctx->request_message_len;

    // ToDo: Fix the amount of data to copy from report to request_message[]
    memcpy(&ctx->request_message[received_packet_count * PKT_SIZE_DEFAULT], report, len);
    received_len += len;
    received_packet_count++;

    // Checking both Packet Count and Message Length since received_len may overflow total_len
    if(received_len == total_len || received_packet_count == total_packet_count)
    {
        received_packet_count = 0;
        LOG_DBG("Message Received completely. ");
        event_queue_push(EVENT_MESSAGE_RECEIVED);
    }

    return CTAPHID_OK;
}

