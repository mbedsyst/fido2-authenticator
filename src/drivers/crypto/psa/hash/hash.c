#include "interfaces/crypto/hash/hash.h"
#include "crypto_status.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(crypto_hash_interface);

int crypto_hash(crypto_hash_alg_t alg,
                const uint8_t *input, 
                size_t input_len,
                uint8_t *digest, 
                size_t *digest_len)
{
	uint32_t olen;
	psa_status_t status;

	LOG_INF("Hashing using SHA256...");

	/* Calculate the SHA256 hash */
	status = psa_hash_compute(alg, 
                              input, 
                              input_len,
                              digest, 
                              digest_len), 
                              &olen);
	if (status != PSA_SUCCESS) 
    {
		LOG_INF("psa_hash_compute failed! (Error: %d)", status);
		return APP_ERROR;
	}

	LOG_INF("%d bytes of Digest generated. Hashing successful!", olen);

	return 0;   
}