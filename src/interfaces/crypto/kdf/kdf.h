#ifndef CRYPTO_KDF_H
#define CRYPTO_KDF_H

#include <stddef.h>
#include <stdint.h>

typedef enum 
{
    CRYPTO_KDF_HKDF_SHA256,
} crypto_kdf_alg_t;

int crypto_kdf_hkdf(crypto_kdf_alg_t alg,
                    const uint8_t *salt, size_t salt_len,
                    const uint8_t *ikm, size_t ikm_len,
                    const uint8_t *info, size_t info_len,
                    uint8_t *okm, size_t okm_len);

#endif // CRYPTO_KDF_H
