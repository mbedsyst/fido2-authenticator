#ifndef CRYPTO_COMMON_H
#define CRYPTO_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file crypto_common.h
 * @brief Common definitions for the crypto interface.
 */

/**
 * @enum crypto_status_t
 * @brief Status codes for crypto operations.
 */
typedef enum {
    CRYPTO_OK = 0,              /**< Operation succeeded */
    CRYPTO_ERR_INVALID_PARAM,   /**< Bad input argument */
    CRYPTO_ERR_UNSUPPORTED,     /**< Unsupported algorithm or mode */
    CRYPTO_ERR_KEY_NOT_FOUND,   /**< Requested key missing */
    CRYPTO_ERR_KEY_INVALID,     /**< Key corrupted or invalid format */
    CRYPTO_ERR_MEMORY,          /**< Memory allocation / buffer issue */
    CRYPTO_ERR_HW_FAILURE,      /**< Hardware / accelerator fault */
    CRYPTO_ERR_INTERNAL         /**< Catch-all for unexpected conditions */
} crypto_status_t;

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_COMMON_H */
