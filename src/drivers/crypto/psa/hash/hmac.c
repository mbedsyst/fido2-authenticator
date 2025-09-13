#include "interfaces/crypto/hash/hmac.h"
#include "crypto_status.h"

int crypto_hmac(crypto_hmac_alg_t alg,
                const uint8_t *key, size_t key_len,
                const uint8_t *input, size_t input_len,
                uint8_t *mac, size_t *mac_len)
{
    (void)alg;
    (void)key;
    (void)key_len;
    (void)input;
    (void)input_len;
    (void)mac;
    (void)mac_len;
    return CRYPTO_ERR_NOT_IMPLEMENTED;
}
