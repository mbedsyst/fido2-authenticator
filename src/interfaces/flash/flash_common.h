#ifndef FLASH_COMMON_H
#define FLASH_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file flash_common.h
 * @brief Common definitions for the flash interface.
 */

/**
 * @enum flash_status_t
 * @brief Status codes for flash operations.
 */

 typedef enum {
    FLASH_OK = 0,
    FLASH_ERR_GENERIC = -1,
    FLASH_ERR_INVALID_ARG = -2,
    FLASH_ERR_NO_DEVICE = -3,
    FLASH_ERR_ALIGN = -4,
    FLASH_ERR_OUT_OF_RANGE = -5,
    FLASH_ERR_ERASE_FAIL = -6,
    FLASH_ERR_WRITE_FAIL = -7,
    FLASH_ERR_READ_FAIL = -8,
    FLASH_ERR_UNSUPPORTED = -9,
    FLASH_ERR_NOT_IMPLEMENTED = -100,
} flash_status_t;

#ifdef __cplusplus
}
#endif

#endif /* FLASH_COMMON_H */
