/**
 * @file    ctaphid_reconstructor.c
 * @brief   Handles reassembly of CTAP2 Payload from INIT and CONT packets
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

ctaphid_status_t ctaphid_msg_reconstructor(uint8_t* message_buffer)
{
    if(message_buffer == NULL)
    {
        LOG_ERR("Received NULL Message");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    static uint8_t payload_buffer[MAX_PAYLOAD_LEN];
    memset(payload_buffer, 0, sizeof(payload_buffer));

    uint16_t total_len = ((uint16_t)message_buffer[INIT_BCNTH_POS] << 8) | message_buffer[INIT_BCNTL_POS];
    uint16_t data_copied = 0;

    uint16_t init_payload_len = (total_len < INIT_DATA_MAX_LEN) ? total_len : INIT_DATA_MAX_LEN;
    memcpy(payload_buffer, &message_buffer[INIT_DATA_POS], init_payload_len);
    data_copied += init_payload_len;

    uint16_t data_remaining = total_len - data_copied;
    uint16_t cont_packet_index = 0;

    while (data_remaining > 0) 
    {
        uint16_t cont_offset = PKT_SIZE_DEFAULT + cont_packet_index * PKT_SIZE_DEFAULT;

        uint16_t copy_len = (data_remaining < CONT_DATA_MAX_LEN) ? data_remaining : CONT_DATA_MAX_LEN;

        memcpy(payload_buffer + data_copied, &message_buffer[cont_offset + 5], copy_len);
        data_copied += copy_len;
        data_remaining -= copy_len;
        cont_packet_index++;
    }
}