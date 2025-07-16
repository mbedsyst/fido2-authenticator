#ifndef INTERFACES_TRANSPORT_IF_H_
#define INTERFACES_TRANSPORT_IF_H_

#include <stddef.h>
#include <stdint.h>

/* Callback invoked when a **complete CTAPHID message** is ready. */
typedef void (*transport_rx_cb_t)(const uint8_t *data, size_t len);

/* Generic transport contract (USB‑HID, BLE, NFC …). */
typedef struct 
{
    /** Initialise backend. Supply callback for RX. */
    int (*init)(transport_rx_cb_t cb);
    /** Send a *single* CTAPHID message (already framed) upstream. */
    int (*send)(const uint8_t *data, size_t len);
} transport_if_t;

#endif /* INTERFACES_TRANSPORT_IF_H_ */