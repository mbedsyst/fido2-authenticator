/**
 * @file    ctaphid_reconstructor.h
 * @brief   Header for CTAPHID Reconstructor
 * 
 * This header includes the Public function header for
 * the CTAPHID Message Reconstructor
 */

#ifndef CORE_CTAPHID_RECONSTRUCTOR_H_
#define CORE_CTAPHID_RECONSTRUCTOR_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Reconstructs a CTAPHID message from a 64-byte USB HID report.
 *
 * This function takes a raw HID report (received from the USB stack) and
 * reconstructs the complete CTAPHID message by assembling INIT and CONT packets.
 * Once the full message is reconstructed, it may be forwarded to the dispatcher.
 *
 * @param[in]  report   Pointer to the received 64-byte HID report.
 *
 * @return Status of the reconstruction process.
 *         - CTAPHID_OK on success.
 *         - Non-zero value on error
 */
ctaphid_status_t ctaphid_msg_reconstructor(uint8_t* report);

#endif