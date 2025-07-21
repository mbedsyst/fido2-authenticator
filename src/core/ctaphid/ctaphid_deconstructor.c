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

// Output buffer must be large enough to hold INIT + CONT packets
size_t ctaphid_construct_packets(uint8_t* payload, uint16_t payload_len, uint32_t cid, uint8_t cmd, uint8_t* out_buf)
{
    size_t total_packets = 0;
    size_t offset = 0;

    // --- INIT Packet ---
    uint8_t* init_pkt = out_buf;
    init_pkt[0] = (cid >> 24) & 0xFF;
    init_pkt[1] = (cid >> 16) & 0xFF;
    init_pkt[2] = (cid >> 8)  & 0xFF;
    init_pkt[3] = (cid)       & 0xFF;

    init_pkt[4] = 0x80 | (cmd & 0x7F);  // Ensure MSB is set for INIT CMD
    init_pkt[5] = (payload_len >> 8) & 0xFF;  // BCNTH
    init_pkt[6] = payload_len & 0xFF;         // BCNTL

    size_t init_data_len = (payload_len > INIT_PAYLOAD_MAX) ? INIT_PAYLOAD_MAX : payload_len;
    memcpy(&init_pkt[7], payload, init_data_len);
    offset += PACKET_SIZE;
    total_packets++;

    size_t remaining = payload_len - init_data_len;
    size_t payload_offset = init_data_len;

    // --- CONT Packets ---
    uint8_t seq = 0;

    while (remaining > 0) 
    {
        uint8_t* cont_pkt = out_buf + offset;

        cont_pkt[0] = (cid >> 24) & 0xFF;
        cont_pkt[1] = (cid >> 16) & 0xFF;
        cont_pkt[2] = (cid >> 8)  & 0xFF;
        cont_pkt[3] = (cid)       & 0xFF;

        cont_pkt[4] = seq++;

        size_t chunk = (remaining > CONT_PAYLOAD_MAX) ? CONT_PAYLOAD_MAX : remaining;
        memcpy(&cont_pkt[5], &payload[payload_offset], chunk);

        payload_offset += chunk;
        remaining -= chunk;
        offset += PACKET_SIZE;
        total_packets++;
    }

    return total_packets;  // Number of 64-byte packets written to out_buf
}