#ifndef CTAP2_COMMANDS_H
#define CTAP2_COMMANDS_H

typedef enum {
    CTAP2_OK = 0,               /**< Operation succeeded */

    CTAP2_ERR_INVALID_CMD,      /**< Command not recognized */
    CTAP2_ERR_INVALID_LEN,      /**< Invalid request length */
    CTAP2_ERR_INVALID_CBOR,     /**< Malformed or invalid CBOR structure */
    CTAP2_ERR_UNSUPPORTED,      /**< Unsupported option/parameter */
    CTAP2_ERR_INVALID_STATE,    /**< Operation not allowed in current state */
    CTAP2_ERR_NO_CREDENTIALS,   /**< No matching credentials found */
    CTAP2_ERR_PIN_INVALID,      /**< Invalid PIN or auth token */
    CTAP2_ERR_OPERATION_DENIED, /**< Policy or user denied operation */
    CTAP2_ERR_TIMEOUT,          /**< Timeout waiting for input/event */
    CTAP2_ERR_INTERNAL          /**< Catch-all for unexpected conditions */
} ctap2_status_t;

#define CTAP2_ERR_INVALID_COMMAND 0x01  // example error code

#endif // CTAP2_COMMANDS_H
