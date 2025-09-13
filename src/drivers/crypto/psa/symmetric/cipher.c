#include "interfaces/crypto/symmetric/cipher.h"
#include "crypto_status.h"

int crypto_cipher_encrypt(uint32_t key_id,
                          crypto_cipher_alg_t alg,
                          const uint8_t *iv, size_t iv_len,
                          const uint8_t *aad, size_t aad_len,
                          const uint8_t *input, size_t input_len,
                          uint8_t *output, size_t *output_len,
                          uint8_t *tag, size_t tag_len)
{
    (void)key_id;
    (void)alg;
    (void)iv;
    (void)iv_len;
    (void)aad;
    (void)aad_len;
    (void)input;
    (void)input_len;
    (void)output;
    (void)output_len;
    (void)tag;
    (void)tag_len;
    return CRYPTO_ERR_NOT_IMPLEMENTED;
}

int crypto_cipher_decrypt(uint32_t key_id,
                          crypto_cipher_alg_t alg,
                          const uint8_t *iv, size_t iv_len,
                          const uint8_t *aad, size_t aad_len,
                          const uint8_t *input, size_t input_len,
                          uint8_t *output, size_t *output_len,
                          const uint8_t *tag, size_t tag_len)
{
    (void)key_id;
    (void)alg;
    (void)iv;
    (void)iv_len;
    (void)aad;
    (void)aad_len;
    (void)input;
    (void)input_len;
    (void)output;
    (void)output_len;
    (void)tag;
    (void)tag_len;
    return CRYPTO_ERR_NOT_IMPLEMENTED;
}
