#ifndef APP_APP_CTX_H_
#define APP_APP_CTX_H_

#include "led_if.h"
#include "button_if.h"
#include "transport_if.h"

#define MAX_MESSAGE_SIZE     8256
#define MAX_PAYLOAD_SIZE     7609

typedef struct
{
    // Interfaces
    const led_if_t *led;
    const button_if_t *button;
    const transport_if_t *transport;

    // Request & Response Buffers
    uint8_t request_message[MAX_MESSAGE_SIZE];      // INIT + CONT
    uint8_t request_payload[MAX_PAYLOAD_SIZE];      // Extracted CBOR payload
    uint8_t response_payload[MAX_PAYLOAD_SIZE]; // CBOR-encoded output
    uint8_t response_message[MAX_MESSAGE_SIZE]; // INIT + CONT fragments

    // Metadata
    uint32_t channel_id;
    uint8_t  cmd;
    uint16_t payload_len;
    uint16_t response_payload_len;
    uint16_t response_message_len;

    // Optionally
    uint16_t bytes_received;
    uint16_t bytes_expected;

} app_ctx_t;

extern app_ctx_t ctx;

void app_ctx_init(void);

#endif
