#include "interfaces/crypto/asymmetric/ecdsa.h"
#include "crypto_status.h"

int crypto_ecdsa_sign(uint32_t key_id,
                      crypto_ecdsa_alg_t alg,
                      const uint8_t *hash, size_t hash_len,
                      uint8_t *sig, size_t *sig_len)
{
    (void)key_id;
    (void)alg;
    (void)hash;
    (void)hash_len;
    (void)sig;
    (void)sig_len;
    return CRYPTO_ERR_NOT_IMPLEMENTED;
}

int crypto_ecdsa_verify(uint32_t key_id,
                        crypto_ecdsa_alg_t alg,
                        const uint8_t *hash, size_t hash_len,
                        const uint8_t *sig, size_t sig_len)
{
    (void)key_id;
    (void)alg;
    (void)hash;
    (void)hash_len;
    (void)sig;
    (void)sig_len;
    return CRYPTO_ERR_NOT_IMPLEMENTED;
}
