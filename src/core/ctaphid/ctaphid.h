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

// CTAPHID Specification Error Codes
#define ERR_INVALID_CMD	        0x01
#define ERR_INVALID_PAR	        0x02
#define ERR_INVALID_LEN	        0x03
#define ERR_INVALID_SEQ	        0x04
#define ERR_MSG_TIMEOUT	        0x05
#define ERR_CHANNEL_BUSY	    0x06
#define ERR_LOCK_REQUIRED	    0x0A
#define ERR_INVALID_CHANNEL	    0x0B
#define ERR_OTHER	            0x7F

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
#define PKT_SEQ_MIN             0
#define PKT_SEQ_MAX             127
#define PKT_SEQ_COUNT           128
#define PKT_SIZE_DEFAULT        64
#define PKT_INIT_MAX_COUNT      1
#define PKT_CONT_MAX_COUNT      128
#define PKT_TOTAL_MAX_COUNT     129

// Message Related Macro Definitions
#define MAX_PAYLOAD_LEN         (7069 + 128)
#define MAX_MESSAGE_LEN         (8256 + 128)

// CTAPHID Command Related Definitions
#define INIT_CMD_NONCE_LEN      0x08
#define CTAPHID_PROTOCOL_VER    0x02
#define MAJOR_DEV_VER_NUMBER    0x00
#define MINOR_DEV_VER_NUMBER    0x00
#define BUILD_DEV_VER_NUMBER    0x00
#define CAPABILITY_WINK         0x01
#define CAPABILITY_CBOR         0x04
#define CAPABILITY_NMSG         0x08
#define STATUS_PROCESSING       0x01
#define STATUS_UP_NEEDED        0x02

// Internal CTAPHID status code enumeration
typedef enum
{
    CTAPHID_OK = 0,
    CTAPHID_ERROR_INVALID_INPUT,
    CTAPHID_ERROR_INVALID_PKT,
    CTAPHID_ERROR_INVALID_CID,
    CTAPHID_ERROR_INVALID_CMD,
    CTAPHID_ERROR_INVALID_SEQ,
    CTAPHID_ERROR_INVALID_LEN,
    CTAPHID_ERROR_TIMEOUT,
    CTAPHID_ERROR_CHANNEL_BUSY,
    CTAPHID_ERROR_OTHER,
} ctaphid_status_t;

#endif