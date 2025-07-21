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
 * @param[in]  report   Pointer to the received 64-byte USB HID report.
 */
void ctaphid_receive_packet(uint8_t* report);

#endif