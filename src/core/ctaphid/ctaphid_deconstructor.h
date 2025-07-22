/**
 * @file    ctaphid_deconstructor.h
 * @brief   Header for CTAPHID Deconstructor
 * 
 * This header includes the Public function header for
 * the CTAPHID Message Deconstructor
 */

 #ifndef CORE_CTAPHID_DECONSTRUCTOR_H_
 #define CORE_CTAPHID_DECONSTRUCTOR_H_
 
 #include <stdbool.h>
 #include <stdint.h>
 
 /**
  * @brief Deconstructs a CTAPHID message from a CTAP2 response payload.
  *
  * This function takes a CTAP2 response payload and deconstructs the complete 
  * CTAPHID message by fragmenting into INIT and CONT packets. Once the full 
  * message is deconstructed, it will be forwarded to the Transport.
  *
  * @param[in]  ctx   Pointer to the Application Context structure.
  *
  * @return Status of the deconstruction process.
  *         - CTAPHID_OK on success.
  *         - Non-zero value on error
  */
ctaphid_status_t ctaphid_payload_deconstructor(app_ctx_t *ctx);
 
 #endif