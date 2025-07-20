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

#include "ctaphid.h"
#include "ctaphid_deconstructor.h"

LOG_MODULE_REGISTER(ctaphid_deconstructor);

extern uint8_t active_cid[CID_LEN];
extern uint8_t active_cmd;

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
ctaphid_status_t ctaphid_msg_deconstructor(uint8_t* response, uint16_t response_len)
{
    if(response == NULL)
    {
        LOG_ERR("Received NULL Report");
        return CTAPHID_ERROR_INVALID_INPUT;
    }

    uint8_t packet_count = 0;
    uint8_t cont_packet_payload_len = 0;
    static ctaphid_resp_session_t resp_session = {0};
    uint8_t packet_array[PKT_SIZE_DEFAULT] = {0};

    memcpy(&resp_session.channel_id, &active_cid, CID_LEN);
    resp_session.cmd = active_cmd;
    resp_session.total_len = response_len;
    resp_session.seq = CalculateSequencePackets(response_len);


    // Sending the INIT Packet
    memcpy(&packet_array, &resp_session.channel_id, CID_LEN);
    packet_array[INIT_CMD_POS] = resp_session.cmd;
    packet_array[INIT_BCNTH_POS] = (resp_session.total_len & 0xFF00) <<8;
    packet_array[INIT_BCNTL_POS] = (resp_session.total_len & 0xFF);
    if(seq == 0)
    {
        memcpy(&packet_array, &response, response_len);
        resp_session.sent_len = response_len;
    }
    else
    {
        memcpy(&packet_array, &response, INIT_DATA_MAX_LEN);
        resp_session.sent_len = INIT_DATA_MAX_LEN;
    }

    // **************************
    // ToDo: Send the INIT Packet
    // **************************

    LOG_DBG("Send INIT Packet");

    resp_session.initialized = true;
    cont_packet_payload_len = response_len - resp_session.sent_len;

    // Sending the CONT Packets
    if(seq != 0)
    {
        for(int seq_count = 0; seq_count < resp_session.seq; seq_count++)
        {
            memset(packet_array, 0, sizeof(packet_array));
            memcpy(&packet_array, &resp_session.channel_id, CID_LEN);
            packet_array[CONT_SEQ_POS] = seq_count;
            if(cont_packet_payload_len < CONT_DATA_MAX_LEN)
            {
                memcpy(&packet_array, &response, cont_packet_payload_len);
                resp_session.sent_len += cont_packet_payload_len;
            }
            else
            {
                memcpy(&packet_array, &response, CONT_DATA_MAX_LEN);
                resp_session.sent_len += CONT_DATA_MAX_LEN;
            }
            
            // **************************
            // ToDo: Send the CONT Packet
            // **************************

            cont_packet_payload_len = response_len - resp_session.sent_len;
        }

        LOG_DBG("Send CONT Packets");
    }

    LOG_DBG("Send out CTAPHID Message");

}