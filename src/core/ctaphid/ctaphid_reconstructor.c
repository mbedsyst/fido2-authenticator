/**
 * @file    ctaphid_reconstructor.c
 * @brief   Handles reassembly of CTAPHID Message from INIT and CONT packets
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

#include "ctaphid.h"
#include "ctaphid_reconstructor.h"

LOG_MODULE_REGISTER(ctaphid_reconstructor);

uint8_t active_cid[CID_LEN] = {0};
uint8_t active_cmd = 0;

static uint8_t CalculateSequencePackets(uint16_t data_len)
{
    if(data_len <= PKT_LEN_FOR_INIT_ONLY)
    {
        LOG_DBG("Payload is only %d bytes. Requires only INIT Packet", data_len);
        return 0;
    }
    else if(data_len > PKT_LEN_FOR_INIT_ONLY && data_len <= PKT_MAX_PAYLOAD_LEN)
    {
        LOG_DBG("Payload is only %d bytes. Requires INIT and CONT packets", data_len);
        uint8_t counter;
        data_len = data_len - 57;
        if(data_len % 59 == 0)
        {
            counter = data_len / 59;
            LOG_DBG("Requires %d Continuation Packets", counter);
            return counter;
        }
        else
        {
            counter = (data_len / 59) + 1;
            LOG_DBG("Requires %d Continuation Packets", counter);
            return counter;
        }
    }
}

ctaphid_status_t ctaphid_msg_reconstructor(uint8_t* report)
{
    if(report == NULL)
    {
        LOG_ERR("Received NULL Report");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    // ToDo: Check and use the following two variables
    static uint8_t packet_counter = 0;
    static uint8_t payload_length = 0;
    static ctaphid_req_session_t req_session = {0};

    // Retrieve the Channel ID from Report
    req_session.channel_id[CID_POS + 0] = report[CID_POS + 0];
    req_session.channel_id[CID_POS + 1] = report[CID_POS + 1];
    req_session.channel_id[CID_POS + 2] = report[CID_POS + 2];
    req_session.channel_id[CID_POS + 3] = report[CID_POS + 3];

    // Checking if it is an INIT Packet
    if(report[INIT_CMD_POS] & 0x80 != 0)
    {
        LOG_DBG("Received an INIT Packet.");

        req_session.cmd = report[INIT_CMD_POS];
        if(req_session.cmd == CTAPHID_INIT)
        {
            LOG_DBG("INIT Command Received");
            // Copy 8-bye Nonce
            // CALL INIT Dispatcher
        }
        else if(req_session.cmd == CTAPHID_PING)
        {
            LOG_DBG("PING Command Received");
            // CALL PING Dispatcher
        }
        else if(req_session.cmd == CTAPHID_CANCEL)
        {
            LOG_DBG("CANCEL Command Received");
            // Call CANCEL Dispatcher
        }
        else if(req_session.cmd == CTAPHID_KEEPALIVE)
        {
            LOG_DBG("KEEPALIVE Command Received");
            // Call KEEPALIVE Dispatcher
        }

        req_session.expected_len = ((report[INIT_BCNTH_POS] << 8) & 0xFF) | (report[INIT_BCNTL_POS] & 0xFF);
        req_session.seq = CalculateSequencePackets(req_session.expected_len);
        // ToDo: Fix bytes to copy from Report
        memcpy(&req_session.data[PKT_INITPKT_DATA_POS], &report[INIT_DATA_POS], DataSizeToCopy);
        // ToDo: Increment the Received Length member carefully
        req_session.received_len += DataSizeToCopy;
        req_session.initialized = true;
        packet_counter++;
    }
    // Checking if it is a CONT Packet
    else if(report[INIT_CMD_POS] & 0x80 == 0)
    {
        // ToDo: Fix bytes to copy from Report
        memcpy(&req_session.data[PKT_CONTPKT_DATA_POS], &report[CONT_DATA_POS], DataSizeToCopy);
        // ToDo: Increment the Received Length member carefully
        req_session.received_len += DataSizeToCopy;
        packet_counter++;
    }
    // Handling if Packet is Invalid
    else
    {
        LOG_ERR("Received an Invalid Packet.");
        return CTAPHID_ERROR_INVALID_PKT;
    }

    if(req_session.received_len == req_session.expected_len)
    {
        LOG_DBG("Received all packets. Total %d bytes", req_session.received_len);
        memcpy(active_cid, &req_session.channel_id, CID_LEN);
        active_cmd = req_session.cmd;

        if(req_session.cmd == CTAPHID_MSG)
        {
            LOG_DBG("MSG Command Received");
            // Call MSG Dispatcher
        }
        else if(req_session.cmd == CTAPHID_CBOR)
        {
            LOG_DBG("CBOR Command Received");
            // Call CBOR Dispatcher
        }
    }
}