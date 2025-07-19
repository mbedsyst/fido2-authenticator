#include "ctaphid.h"

/*
Offset  Length	Mnemonic	Description
0	    4	    CID	        Channel identifier
4	    1	    CMD	        Command identifier (bit 7 always set)
5	    1	    BCNTH	    High part of payload length
6	    1	    BCNTL	    Low part of payload length
7	    57	    DATA	    Payload data 

Offset	Length	Mnemonic	Description
0	    4	    CID	        Channel identifier
4	    1	    SEQ	        Packet sequence 0x00..0x7f (bit 7 always cleared)
5	    59	    DATA	    Payload data 
*/

/*
    typedef struct 
    {
        uint8_t channel_id[4];
        uint8_t cmd;
        uint8_t data[7609];
        uint16_t expected_len;
        uint16_t received_len;
        uint8_t seq;
        bool initialized;
    } ctaphid_req_session_t;

    1. ctaphid_request_packet(uint8_t * hid_report) [Packet Receive function]
        * Take as argument the pointer to the 64 byte HID Report from the hook
        * Create the structure variable
        * Retrieve and store the 4-byte CID 
        * Check if the packet is INIT (byte[4] & 0x80 != 0)
            * Check Command ID to see if is CTAPHID_CANCEL or CTAPHID_KEEPALIVE
                * Process Response for CANCEL and KEEPALIVE commands, and return
            * Store the Command ID in the struct member 'cmd'
            * Retrieve the Packet Length from HID Report
            * Calculate & Set Packet Count to struct member 'seq'
            * Set Packet Length to the struct member 'expected_len'
            * Copy Payload from packet to struct member 'data'
            * Increment Received Payload size in struct member 'received_len'
            * Set the struct member 'initialized' to true
            * Set Fragment counter (static) to 0
        * Check if the packet is CONT (byte[4] & 0x80 == 0)
            * Copy Payload from packet to struct member 'data'
            * Increment Received Payload size in struct member 'received_len'
            * Increment Fragment counter by 1
        * Check if received_len = expected_len
            * Set Channel ID to a global variable 'active_channel_ID'
            * Set Command ID to a global variable 'active_command_ID'
            * If true, call CTAPHID Dispatcher
        * Return status (to be checked) 

    typedef struct 
    {
        uint8_t channel_id[4];     // From session
        uint8_t cmd;               // Response command (0x80 | original cmd)
        const uint8_t *data;       // Pointer to full payload
        uint16_t total_len;        // Total payload length
        uint16_t sent_len;         // Number of bytes already sent
        uint8_t seq;               // Current sequence number for CONT
        bool initialized;          // Transmission in progress
    } ctaphid_resp_session_t;

    2. Packet Response Function
        * Take as argument the pointer to the CTAP2 'Command + Payload' array
        * Create the structure variable
        * Set the Channel ID in the struct member 'cid'
        * Set the Command ID in the struct member 'cmd'
        * Set the Payload length in the struct member 'total_len'
        * Calculate & Set Packet count to struct member 'seq'
        * Create a 64-byte array of type uint8_t and set to zero
        * Send INIT Packet
            * Set the 4-byte CID in the array
            * Set the 1-byte CMD in the array
            * Set the 2-byte LEN in the array
            * Copy 57 byte of data from the payload referenced by 'data'
            * Call the ctx.transport->send() to send out the INIT packet
        * Send CONT Packets (for (int i = 0; i < seq; i++))
            * Set the 4-byte CID in the array
            * Set the 1-byte SEQ in the array
            * Copy 59 byte of data from the payload referenced by 'data'
            * Call the ctx.transport->send() to send out the CONT packets
        * Return status (to be checked)

    2. Dispatcher function
        * Take as argument the pointer to the CTAPHID Session struct variable
        * Read the 
    3. Deconstructor function
*/