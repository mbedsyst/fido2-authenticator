#include "interfaces/crypto/kdf/kdf.h"
#include "crypto_status.h"

int crypto_kdf(crypto_kdf_alg_t alg,
               const uint8_t *salt, size_t salt_len,
               const uint8_t *ikm, size_t ikm_len,
               const uint8_t *info, size_t info_len,
               uint8_t *okm, size_t okm_len)
{
    (void)alg;
    (void)salt;
    (void)salt_len;
    (void)ikm;
    (void)ikm_len;
    (void)info;
    (void)info_len;
    (void)okm;
    (void)okm_len;
    return CRYPTO_ERR_NOT_IMPLEMENTED;
}
