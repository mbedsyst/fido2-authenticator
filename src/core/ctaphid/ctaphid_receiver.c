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
#include "ctaphid_receiver.h"

uint8_t ctaphid_message[MSG_MAX_LEN] = {0};

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
    else
    {
        // Handle Invalid Payload Length
    }
}

void ctaphid_receive_packet(uint8_t* report)
{
    uint8_t channel_id[CID_LEN] = {0};
    uint8_t active_CID[CID_LEN] = {0};
    uint8_t broadcast_cid[CID_LEN] = {0xFF, 0xFF, 0xFF, 0xFF};

    static uint16_t payload_length = 0;
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
        else if(report[INIT_CMD_POS] == CTAPHID_KEEPALIVE)
        {
            /*** Process KEEPALIVE ***/
        }
        else if(report[INIT_CMD_POS] == CTAPHID_INIT)
        {
            if(memcmp(channel_id, broadcast_cid, CID_LEN) == 0)
            {
                /*** Process INIT NEW CID ***/
            }
            else if(memcmp(channel_id, active_CID, CID_LEN) == 0)
            {
                /*** Process INIT SAME CID ***/
            }
        }

        payload_length = ((report[INIT_BCNTH_POS]) << 8) | (report[INIT_BCNTL_POS]);
        total_len = payload_length;
        total_packet_count = CalculatePacketCount(total_len);
    }

    memcpy(&ctaphid_message[], report, PKT_SIZE_DEFAULT);
    received_len += PKT_SIZE_DEFAULT;
    received_packet_count += 1;

    if(received_len == total_len)
    {
        /*** Set FSM State to PROCESSING ***/
    }

    





}

