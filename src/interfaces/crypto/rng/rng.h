#ifndef CRYPTO_RNG_H
#define CRYPTO_RNG_H

#include <stddef.h>
#include <stdint.h>

/**
 * Generate cryptographically secure random bytes.
 *
 * @param output    Buffer to fill with random data.
 * @param output_len Number of bytes to generate.
 */
int crypto_rng(uint8_t *output, size_t output_len);

#endif // CRYPTO_RNG_H
