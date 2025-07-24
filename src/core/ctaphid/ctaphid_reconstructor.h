/**
 * @file    ctaphid_reconstructor.h
 * @brief   Header for CTAPHID Reconstructor
 * 
 * This header includes the Public function header for
 * the CTAPHID Payload Reconstructor
 */

 #ifndef CORE_CTAPHID_RECONSTRUCTOR_H_
 #define CORE_CTAPHID_RECONSTRUCTOR_H_

 #include <stdbool.h>
 #include <stdint.h>

 #include "app_ctx.h"

/**
 * @brief Reconstructs the CTAPHID Payload from the Request Message Buffer.
 *
 * This function takes the Request Message Buffer and reconstructs the complete 
 * CTAP2 payload by parsing the INIT and CONT packets. Once the full payload is 
 * reconstructed, it may be forwarded to the CTAPHID dispatcher.
 *
 * @param[in]  ctx   Pointer to the Application Context structure.
 *
 * @return Status of the Payload Reconstruction process.
 *         - CTAPHID_OK on success.
 *         - Non-zero value on error
 */
ctaphid_status_t ctaphid_payload_reconstructor(app_ctx_t *ctx);

#endif