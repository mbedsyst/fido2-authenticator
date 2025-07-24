/**
 * @file    ctaphid_deconstructor.h
 * @brief   Header for CTAPHID Deconstructor
 * 
 * This header includes the Public function header for
 * the CTAPHID Payload Deconstructor
 */

 #ifndef CORE_CTAPHID_DECONSTRUCTOR_H_
 #define CORE_CTAPHID_DECONSTRUCTOR_H_
 
 #include <stdbool.h>
 #include <stdint.h>
 
 #include "app_ctx.h"

/**
 * @brief Deconstructs the CTAPHID Payload into the Response Message Buffer.
 *
 * This function takes a CTAP2 Response Payload and Deconstructs it into a 
 * complete CTAPHID message by fragmenting it into INIT and CONT packets. 
 * Once the full message is Deconstructed, it will be forwarded to the 
 * Transport Handler.
 *
 * @param[in]  ctx   Pointer to the Application Context structure.
 *
 * @return Status of the Payload Deconstruction process.
 *         - CTAPHID_OK on success.
 *         - Non-zero value on error
 */
ctaphid_status_t ctaphid_payload_deconstructor(app_ctx_t *ctx);
 
 #endif