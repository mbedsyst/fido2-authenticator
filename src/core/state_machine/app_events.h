#ifndef CORE_STATE_MACHINE_APP_EVENTS_H_
#define CORE_STATE_MACHINE_APP_EVENTS_H_

typedef enum
{
    EVENT_REQUEST_RECEIVING_STARTED,
    EVENT_REQUEST_RECEIVING_FINISHED,
    EVENT_PAYLOAD_RECONSTRUCTED,
    EVENT_PROCESSING_DONE,
    EVENT_PAYLOAD_DECONSTRUCTED,
    EVENT_RESPONSE_SENT,
    EVENT_ERROR_OCCURED
} app_event_t;

#endif