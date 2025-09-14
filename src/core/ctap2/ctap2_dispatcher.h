#ifndef CTAP2_DISPATCHER_H
#define CTAP2_DISPATCHER_H

#include <stdint.h>
#include "app_ctx.h"   // <-- where app_ctx_t is defined

/**
 * @brief Initialize the CTAP2 dispatcher (if needed).
 */
void ctap2_dispatcher_init(void);

/**
 * @brief Process a CTAP2 request.
 *
 * This function inspects the CTAP2 command byte from the request payload,
 * dispatches it to the correct handler, and writes the response payload
 * back into the application context.
 *
 * @param ctx Pointer to the global application context.
 * @return 0 on success, negative error code on failure.
 */
int ctap2_dispatch_request(app_ctx_t *ctx);

#endif // CTAP2_DISPATCHER_H
