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
#include "app_events.h"
#include "event_queue.h"
#include "ctaphid_receiver.h"

uint16_t calculate_message_size(uint16_t req_payload_len)
{
    if(req_payload_len <= INIT_DATA_MAX_LEN)
    {
        return (req_payload_len + 7);
    }
    else if (req_payload_len > INIT_DATA_MAX_LEN && req_payload_len <= PKT_MAX_PAYLOAD_LEN)
    {
        uint8_t cont_payload_len, cont_packet_count;
        cont_payload_len = req_payload_len - INIT_DATA_MAX_LEN;
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

uint8_t calculate_packet_count(uint16_t req_payload_len)
{
    if(req_payload_len <= INIT_DATA_MAX_LEN)
    {
        return 1;
    }
    else if (req_payload_len > INIT_DATA_MAX_LEN && req_payload_len <= PKT_MAX_PAYLOAD_LEN)
    {
        uint8_t count, seq_count;
        count = req_payload_len - INIT_DATA_MAX_LEN;
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

static bool is_init_packet(uint8_t *report)
{
    return (report[INIT_CMD_POS] & 0x80 != 0);
}

static uint32_t extract_cid(uint8_t *report)
{
    uint32_t cid;
    cid = (report[CID_POS + 0] << 24) | (report[CID_POS + 1] << 16) | (report[CID_POS + 2] << 8) | (report[CID_POS + 3] << 0);
    return cid;
}

static uint8_t extract_cmd(uint8_t *report)
{
    return (report[INIT_CMD_POS]);
}

static extract_seq(uint8_t *report)
{
    return (report[CONT_SEQ_POS]);
}

static uint16_t extract_payload_len(uint8_t *report)
{
    return (((report[INIT_BCNTH_POS]) << 8) | (report[INIT_BCNTL_POS]));
}

ctaphid_status_t ctaphid_receive_packet(app_ctx_t *ctx, uint8_t *report, uint8_t len)
{
    uint32_t incoming_cid = extract_cid(report);
    uint8_t cmd = extract_cmd(report);
    bool is_init_pkt = is_init_packet(report);

    // Checking if Device is in IDLE state
    if (ctx->device_state == STATE_IDLE) 
    {
        // Checking if it is an INIT Packet with INIT Command
        if (is_init_pkt && cmd == CTAPHID_INIT) 
        {
            ctx->active_request_cid = incoming_cid;
            ctx->request_cmd = cmd;
            ctx->request_payload_len = INIT_NONCE_LEN;
            memcpy(ctx->request_payload, report[INIT_DATA_POS], INIT_NONCE_LEN);
            event_queue_push(EVENT_PAYLOAD_RECONSTRUCTED);
            return CTAPHID_OK;
        }
        // Checking if it is an INIT Packet with CANCEL Command
        else if (is_init_pkt && cmd == CTAPHID_CANCEL)
        {
            event_queue_push(EVENT_RESPONSE_SENT);
            return CTAPHID_OK;
        }
        // Checking if it is an INIT Packet with MSG/CBOR/PING Command
        else if (is_init_pkt && ((cmd == CTAPHID_MSG) || (cmd == CTAPHID_CBOR) || (cmd == CTAPHID_PING)))
        {
            ctx->request_payload_len = extract_payload_len(report);
            ctx->request_message_len = calculate_message_size(ctx->request_payload_len);
            ctx->request_packet_count = calculate_packet_count(ctx->request_payload_len);
            // ToDo: Research and fix the size of data to copy
            memcpy(ctx->request_message[CID_POS], report, len);
            ctx->request_message_copied_len += len;
            event_queue_push(EVENT_REQUEST_RECEIVING_STARTED);
            return CTAPHID_OK;
        }
        // If the packet is a CONT Packet
        else 
        {
            ctx->non_active_incoming_cid = incoming_cid;
            // ToDo: Add CTAPHID Specification Error codes in Header
            ctx->remapped_error_code = ERR_INVALID_PAR;
            // ToDo: Setup the Fast Responder Thread & Queue
            // Push EVENT_ERROR_HANDLE to fast responder queue
            return CTAPHID_OK;
        }
    }
    // If the device is not in IDLE state
    else 
    {
        // Checking if the Retrieved CID is the Active CID
        if (incoming_cid == ctx->active_request_cid) 
        {
            // Checking if it is an INIT Packet with INIT Command
            if (is_init_pkt && cmd == CTAPHID_INIT) 
            {
                ctx->non_active_incoming_cid = incoming_cid;
                // ToDo: Add CTAPHID Specification Error codes in Header
                ctx->remapped_error_code = ERR_CHANNEL_BUSY;
                // ToDo: Setup the Fast Responder Thread & Queue
                // Push EVENT_ERROR_HANDLE to fast responder queue
                return CTAPHID_OK;
            } 
            // Checking if it is an INIT Packet with CANCEL Command
            else if (is_init_pkt && cmd == CTAPHID_CANCEL) 
            {
                ctx->abort_requested = true;
                // ToDo: Fix Event enumeration to add EVENT_PROCESS_CANCELLED
                // Push EVENT_PROCESS_CANCELLED to normal queue
                return CTAPHID_OK
            } 
            // If the packet is a CONT Packet
            else 
            {
                ctx->request_sequence_count = extract_seq(report);
                // ToDo: Research and fix the size of data to copy
                memcpy(&ctx->request_message[PKT_SIZE_DEFAULT * (ctx->request_sequence_count + 1)], report, len);
                return CTAPHID_OK;
            }
        } 
        // If the Retrieved CID is not the Active CID
        else 
        {
            // Checking if it is an INIT Packet with INIT/MSG/CBOR/PING Command
            if (is_init_pkt && cmd == CTAPHID_INIT) 
            {
                ctx->non_active_incoming_cid = incoming_cid;
                // ToDo: Add CTAPHID Specification Error codes in Header
                ctx->remapped_error_code = ERR_CHANNEL_BUSY;
                // ToDo: Setup the Fast Responder Thread & Queue
                // Push EVENT_ERROR_HANDLE to fast responder queue
                return CTAPHID_OK;

            } 
            // If the packet is a CONT Packet
            else 
            {
                ctx->non_active_incoming_cid = incoming_cid;
                // ToDo: Add CTAPHID Specification Error codes in Header
                ctx->remapped_error_code = ERR_INVALID_PAR;
                // ToDo: Setup the Fast Responder Thread & Queue
                // Push EVENT_ERROR_HANDLE to fast responder queue
                return CTAPHID_OK;
            }
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

            memcpy(ctx->init_command_nonce, &report[INIT_DATA_POS], sizeof(ctx->init_command_nonce));

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

    ctx->request_message_len = calculate_message_size(ctx->request_payload_len);
    total_packet_count = calculate_packet_count(ctx->request_payload_len);
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

