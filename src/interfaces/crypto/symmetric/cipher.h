#ifndef CRYPTO_CIPHER_H
#define CRYPTO_CIPHER_H

#include <stddef.h>
#include <stdint.h>

int crypto_cipher_encrypt(const uint8_t *key, size_t key_len
                          const uint8_t *iv, size_t iv_len,
                          const uint8_t *aad, size_t aad_len,
                          const uint8_t *input, size_t input_len,
                          uint8_t *output, size_t *output_len,
                          uint8_t *tag, size_t tag_len);

int crypto_cipher_decrypt(const uint8_t *key, size_t key_len,
                          const uint8_t *iv, size_t iv_len,
                          const uint8_t *aad, size_t aad_len,
                          const uint8_t *input, size_t input_len,
                          uint8_t *output, size_t *output_len,
                          const uint8_t *tag, size_t tag_len);

#endif // CRYPTO_CIPHER_H
