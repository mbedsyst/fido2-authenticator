#ifndef APP_APP_CTX_H_
#define APP_APP_CTX_H_

#include "led_if.h"
#include "button_if.h"
#include "transport_if.h"
#include "state_machine.h"

#define MAX_MESSAGE_SIZE     8256
#define MAX_PAYLOAD_SIZE     7609

typedef struct
{
    // Interfaces
    const led_if_t *led;
    const button_if_t *button;
    const transport_if_t *transport;

    // Request & Response Buffers
    uint8_t request_message[MAX_MESSAGE_SIZE];      // Request INIT and CONT packets
    uint8_t request_payload[MAX_PAYLOAD_SIZE];      // Extracted CBOR input payload
    uint8_t response_payload[MAX_PAYLOAD_SIZE];     // Processed CBOR output payload
    uint8_t response_message[MAX_MESSAGE_SIZE];     // Response INIT and CONT packets

    // State Machine Information
    app_state_t device_state;
    uint8_t error_code;
    bool initialized;

    // Transaction Metadata
    uint32_t request_channel_id;
    uint8_t  request_cmd;
    uint16_t request_message_len;
    uint16_t request_payload_len;
    uint8_t request_packet_count;
    uint16_t response_payload_len;
    uint16_t response_message_len;
    uint8_t response_packet_count;

} app_ctx_t;

extern app_ctx_t ctx;

void app_ctx_init(void);

#endif
