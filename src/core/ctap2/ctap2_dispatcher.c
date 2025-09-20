#include "dispatcher.h"
#include "commands.h"  // central enum of CTAP2 commands

// Include each command handler header
#include "features/authenticator/get_info.h"
#include "features/authenticator/make_credential.h"
#include "features/authenticator/get_assertion.h"
#include "features/authenticator/get_next_assertion.h"
#include "features/authenticator/client_pin.h"
#include "features/authenticator/cancel.h"
#include "features/authenticator/reset.h"

#include <string.h>

// ---------------------------------------------------------------------------
// Dispatcher Implementation
// ---------------------------------------------------------------------------

void ctap2_dispatcher_init(void) {
    // For now, nothing needed. Placeholder if dispatcher state is required.
}

ctap2_status_t ctap2_dispatch_request(app_ctx_t *ctx)
{
    if (!ctx || ctx->request_payload_len == 0) {
        return -1; // invalid input
    }

    uint8_t cmd = ctx->request_payload[0];
    const uint8_t *cbor_data = &ctx->request_payload[1];
    size_t cbor_len = ctx->request_payload_len - 1;

    int ret = -2; // default: unknown command

    switch (cmd) {
    case CTAP2_CMD_MAKE_CREDENTIAL:
        ret = ctap2_make_credential_handler(ctx, cbor_data, cbor_len);
        break;

    case CTAP2_CMD_GET_INFO:
        ret = ctap2_get_info_handler(ctx, cbor_data, cbor_len);
        break;

    case CTAP2_CMD_GET_ASSERTION:
        ret = ctap2_get_assertion_handler(ctx, cbor_data, cbor_len);
        break;

    case CTAP2_CMD_GET_NEXT_ASSERTION:
        ret = ctap2_get_next_assertion_handler(ctx, cbor_data, cbor_len);
        break;

    case CTAP2_CMD_CLIENT_PIN:
        ret = ctap2_client_pin_handler(ctx, cbor_data, cbor_len);
        break;

    case CTAP2_CMD_CANCEL:
        ret = ctap2_cancel_handler(ctx, cbor_data, cbor_len);
        break;

    case CTAP2_CMD_RESET:
        ret = ctap2_reset_handler(ctx, cbor_data, cbor_len);
        break;

    default:
        // Unknown CTAP2 command
        ctx->remapped_error_code = CTAP2_ERR_INVALID_COMMAND;
        ret = -3;
        break;
    }

    return ret;
}
