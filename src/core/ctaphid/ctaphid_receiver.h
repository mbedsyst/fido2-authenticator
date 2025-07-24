/**
 * @file    ctaphid_receiver.h
 * @brief   Header for CTAPHID Receiver
 * 
 * This header includes the Public function header for
 * the CTAPHID Message Receiver
 */

#ifndef CORE_CTAPHID_CTAPHID_RECEIVER_H_
#define CORE_CTAPHID_CTAPHID_RECEIVER_H_

/**
 * @brief Receives the USB HID report to form a complete CTAPHID Message.
 *
 * This function takes a raw USB HID report (received from the USB stack) and
 * stores it into a buffer that can be used to reconstruct the CTAPHID message 
 * by parsing the INIT and CONT packets. 
 * 
 * @param[in]  ctx   Pointer to the Application Context structure
 * @param[in]  report   Pointer to the received 64-byte USB HID report.
 * @param[in]  report   Size of valid data read from Endpoint Buffer
 * 
 * @return  Status of the Packet Receiving operation.
 *         - CTAPHID_OK on success.
 */
ctaphid_status_t ctaphid_receive_packet(app_ctx_t *ctx, uint8_t *report, uint8_t len);

#endif