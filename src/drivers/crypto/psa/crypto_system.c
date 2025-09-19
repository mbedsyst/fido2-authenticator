#include "crypto_system.h"

int crypto_system_init(void)
{
    psa_status_t status = psa_crypto_init();
    return (status == PSA_SUCCESS) ? CRYPTO_SUCCESS : CRYPTO_ERROR;
}
