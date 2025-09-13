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
#include "fast_path_responder.h"
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
            ctx->request_payload_len = INIT_CMD_NONCE_LEN;
            memcpy(ctx->request_message, report, INIT_CMD_NONCE_LEN);
            event_queue_push(EVENT_REQUEST_RECEIVING_FINISHED);
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
            LOG_WRN("Received a CONT Packet when system is IDLE");
            LOG_INF("Responding via Fast Path Responder");
            fast_responder_enqueue(incoming_cid, 0x00, ERR_INVALID_PAR);
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
                LOG_WRN("Received an INIT Packet when system is currently processing the same CID");
                LOG_INF("Responding via Fast Path Responder");
                fast_responder_enqueue(incoming_cid, 0x00, ERR_CHANNEL_BUSY);
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
                LOG_WRN("Received an INIT Packet when system is currently processing another CID");
                LOG_INF("Responding via Fast Path Responder");
                fast_responder_enqueue(incoming_cid, 0x00, ERR_CHANNEL_BUSY);
                return CTAPHID_OK;

            } 
            // If the packet is a CONT Packet
            else 
            {
                LOG_WRN("Received a CONT Packet when system is currently processing another CID");
                LOG_INF("Responding via Fast Path Responder");
                fast_responder_enqueue(incoming_cid, 0x00, ERR_INVALID_PAR);
                return CTAPHID_OK;
            }
        }
    }
}