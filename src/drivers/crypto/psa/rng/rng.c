#include "interfaces/crypto/rng/rng.h"
#include "crypto_status.h"

int crypto_rng(uint8_t *output, size_t output_len)
{
    (void)output;
    (void)output_len;
    return CRYPTO_ERR_NOT_IMPLEMENTED;
}
