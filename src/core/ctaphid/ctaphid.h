/**
 * @file    ctaphid.h
 * @brief   Definitions of Macros and Status Codes
 * 
 * This header defines the Macros and Typedef for Return status
 * used in the CTAPHID Layer
 */

#ifndef CORE_CTAPHID_H_
#define CORE_CTAPHID_H_

#include <stdint.h>
#include <stdbool.h>

// Mandatory CTAPHID Commands
#define CTAPHID_MSG             0x03
#define CTAPHID_CBOR            0x10
#define CTAPHID_INIT            0x06
#define CTAPHID_PING            0x01
#define CTAPHID_CANCEL          0x11
#define CTAPHID_ERROR           0x3F
#define CTAPHID_KEEPALIVE       0x3B

// Optional CTAPHID Commands
#define CTAPHID_WINK            0x08
#define CTAPHID_LOCK            0x04

// Packet Independent Macro definitions
#define CID_POS                 0
#define CID_LEN                 4

// Initialization Packet Macro definitions
#define INIT_CID_POS            CID_POS
#define INIT_CMD_POS            4
#define INIT_BCNTH_POS          5
#define INIT_BCNTL_POS          6
#define INIT_DATA_POS           7
#define INIT_CID_LEN            CID_LEN
#define INIT_CMD_LEN            1
#define INIT_BCNT_LEN           2
#define INIT_DATA_MAX_LEN       57

// Continuation Packet Macro definitions
#define CONT_CID_POS            CID_POS
#define CONT_SEQ_POS            4
#define CONT_DATA_POS           5
#define CONT_CID_LEN            CID_LEN
#define CONT_SEQ_LEN            1
#define CONT_DATA_MAX_LEN       59

// Packet Related Macro Definitions
#define PKT_INITPKT_DATA_POS    0
#define PKT_CONTPKT_DATA_POS    57
#define PKT_LEN_FOR_INIT_ONLY   57
#define PKT_SEQ_MIN             0
#define PKT_SEQ_MAX             127
#define PKT_SEQ_COUNT           128
#define PKT_MAX_PAYLOAD_LEN     7069

typedef enum
{
    CTAPHID_OK = 0,
    CTAPHID_ERROR_INVALID_INPUT,
    CTAPHID_ERROR_INVALID_PKT,
    CTAPHID_ERROR_INVALID_CID,
    CTAPHID_ERROR_INVALID_CMD,
    CTAPHID_ERROR_INVALID_SEQ,
    CTAPHID_ERROR_LEN_MISMATCH,
} ctaphid_status_t;

typedef struct 
{
    uint8_t channel_id[CID_LEN];
    uint8_t cmd;
    uint8_t data[PKT_MAX_PAYLOAD_LEN];
    uint16_t expected_len;
    uint16_t received_len;
    uint8_t seq;
    bool initialized;
} ctaphid_req_session_t;

typedef struct 
{
    uint8_t channel_id[CID_LEN];     // From session
    uint8_t cmd;               // Response command (0x80 | original cmd)
    const uint8_t *data;       // Pointer to full payload
    uint16_t total_len;        // Total payload length
    uint16_t sent_len;         // Number of bytes already sent
    uint8_t seq;               // Current sequence number for CONT
    bool initialized;          // Transmission in progress
} ctaphid_resp_session_t;

#endif