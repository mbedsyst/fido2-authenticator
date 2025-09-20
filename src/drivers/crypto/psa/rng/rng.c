#include "interfaces/crypto/rng/rng.h"
#include "interfaces/crypto/crypto_common.h"
#include "crypto_status.h"

#include <stdio.h>
#include <stdlib.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>

LOG_MODULE_REGISTER(crypto_rng_interface);

crypto_status_t crypto_rng(uint8_t *output, size_t output_len)
{
    psa_status_t status = psa_generate_random(output, output_len);
    if (status != PSA_SUCCESS) 
    {
        LOG_INF("psa_generate_random failed! (Error: %d)", status);
        return -1;
    }
    
    return 0;
}
