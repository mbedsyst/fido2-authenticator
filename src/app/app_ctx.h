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
    // Plugged-In Interfaces
    const led_if_t *led;                                // Pointer to LED Interface abstraction
    const button_if_t *button;                          // Pointer to Button Interface abstraction
    const transport_if_t *transport;                    // Pointer to Transport Interface abstraction

    // Device State Information
    app_state_t device_state;                           // Current Device State
    uint8_t     internal_error_code;                    // Error Code internal to Application
    uint8_t     internal_error_domain;                  // Error Domain internal to Application
    uint8_t     remapped_error_code;                    // Error Code remapped to CTAPHID Specification
    bool        initialized;                            // Flag for received valid INIT Packet

    // Request Message Information
    uint32_t    active_request_cid;                     // Active Request's Channel ID
    uint8_t     request_cmd;                            // Active Request's Request CTAPHID Command
    uint16_t    request_payload_len;                    // Request Payload Length in bytes
    uint16_t    request_message_len;                    // Request Message Length (includes headers)
    uint8_t     request_packet_count;                   // Request Packet Counts (INIT + CONT)
    uint8_t     request_sequence_count;                 // Request CONT Packets Sequence Counter
    uint8_t     request_payload[MAX_PAYLOAD_SIZE];      // Reconstructed Input CBOR Payload 
    uint8_t     request_message[MAX_MESSAGE_SIZE];      // Buffered Request INIT and CONT packets

    // Response Message Information
    uint32_t    active_response_cid;                    // Active Response's Channel ID
    uint8_t     response_cmd;                           // Active Response's CTAPHID Command
    uint16_t    response_payload_len;                   // Response Payload Length in bytes
    uint16_t    response_message_len;                   // Response Message Length (includes headers)
    uint8_t     response_packet_count;                  // Response Packet Counts (INIT + CONT)
    uint8_t     response_payload[MAX_PAYLOAD_SIZE];     // Processed Output CBOR payload
    uint8_t     response_message[MAX_MESSAGE_SIZE];     // Buffered Response INIT and CONT packets

    // Interrupted Request Information
    volatile bool abort_requested;                      // Flag to check the Abort Request
    uint32_t    non_active_incoming_cid;                // Non-Active CID that interrupted current process
    uint32_t    new_generated_cid;                      // Newly Generated CID by Device for Client
    uint8_t     init_command_nonce[8];                  // Buffer to hold 8-byte Nonce to echo
} app_ctx_t;

extern app_ctx_t ctx;

void app_ctx_init(void);

#endif
