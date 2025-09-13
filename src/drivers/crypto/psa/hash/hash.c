#include "interfaces/crypto/hash/hash.h"
#include "crypto_status.h"

int crypto_hash(crypto_hash_alg_t alg,
                const uint8_t *input, size_t input_len,
                uint8_t *digest, size_t *digest_len)
{
    (void)alg;
    (void)input;
    (void)input_len;
    (void)digest;
    (void)digest_len;
    return CRYPTO_ERR_NOT_IMPLEMENTED;
}
