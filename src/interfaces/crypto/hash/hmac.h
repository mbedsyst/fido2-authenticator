#ifndef CRYPTO_HMAC_H
#define CRYPTO_HMAC_H

#include <stddef.h>
#include <stdint.h>

#include "crypto_common.h"

typedef enum 
{
    CRYPTO_HMAC_SHA256,
} crypto_hmac_alg_t;


crypto_status_t crypto_hmac_sign(crypto_hmac_alg_t alg,
                       const uint8_t *key, 
                       size_t key_len,
                       const uint8_t *input, 
                       size_t input_len,
                       uint8_t *mac, 
                       size_t *mac_len)

crypto_status_t crypto_hmac_verify(crypto_hmac_alg_t alg,
                        const uint8_t *key, 
                        size_t key_len,
                        const uint8_t *input, 
                        size_t input_len,
                        uint8_t *mac, 
                        size_t *mac_len)

#endif // CRYPTO_HMAC_H
