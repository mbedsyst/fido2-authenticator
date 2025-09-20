#include "interfaces/crypto/kdf/kdf.h"
#include "interfaces/crypto/crypto_common.h"
#include "crypto_status.h"

#include <stdio.h>
#include <stdlib.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>

LOG_MODULE_REGISTER(crypto_kdf_interface);

static int import_input_key(const uint8_t *ikm, size_t ikm_len, psa_key_id_t *input_key_id)
{
	psa_status_t status;
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	/* Configure the input key attributes */
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_DERIVE);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_HKDF(PSA_ALG_SHA_256));
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_DERIVE);
	psa_set_key_bits(&key_attributes, ikm_len * 8);

	/* Import the master key into the keystore */
	status = psa_import_key(&key_attributes,
							ikm,
							ikm_len,
							input_key_id);
	if (status != 0) 
	{
		LOG_INF("psa_import_key failed! (Error: %d)", status);
		return -1;
	}

	return 0;
}

static int export_output_key(uint8_t *okm, size_t okm_len, psa_key_id_t *output_key_id)
{
	size_t olen;
	psa_status_t status;

	/* Export the generated key content to verify it's value */
	status = psa_export_key(output_key_id, okm, okm_len, &olen);
	if (status != 0) 
	{
		LOG_INF("psa_export_key failed! (Error: %d)", status);
		return -1;
	}

	return 0;
}

static int derive_hkdf(psa_key_id_t *input_key_id, psa_key_id_t *output_key_id, 
					   const uint8_t *salt, size_t salt_len,
					   const uint8_t *ikm, size_t ikm_len, 
					   const uint8_t *info, size_t info_len,
					   size_t okm_len)
{
	psa_status_t status;
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_key_derivation_operation_t operation = PSA_KEY_DERIVATION_OPERATION_INIT;

	LOG_INF("Deriving a key using HKDF and SHA256...");

	/* Derived key settings
	 * WARNING: This key usage makes the key exportable which is not safe and
	 * is only done to demonstrate the validity of the results. Please do not use
	 * this in production environments.
	 */
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_RAW_DATA);
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_EXPORT); /* DONT USE IN PRODUCTION */
	psa_set_key_bits(&key_attributes, okm_len * 8);

	/* Set the derivation algorithm */
	status = psa_key_derivation_setup(&operation, PSA_ALG_HKDF(PSA_ALG_SHA_256));
	if (status != 0) 
	{
		LOG_INF("psa_key_derivation_setup failed! (Error: %d)", status);
		return -1;
	}

	/* Set the salt for the operation */
	status = psa_key_derivation_input_bytes(&operation,
											PSA_KEY_DERIVATION_INPUT_SALT,
											salt,
											salt_len);
	if (status != 0) 
	{
		LOG_INF("psa_key_derivation_input_bytes failed! (Error: %d)", status);
		return -1;
	}

	/* Set the master key for the operation */
	status = psa_key_derivation_input_key(&operation, PSA_KEY_DERIVATION_INPUT_SECRET, input_key_id);
	if (status != 0) 
	{
		LOG_INF("psa_key_derivation_input_key failed! (Error: %d)", status);
		return -1;
	}

	/* Set the additional info for the operation */
	status = psa_key_derivation_input_bytes(&operation,
											PSA_KEY_DERIVATION_INPUT_INFO,
											info,
											info_len);
	if (status != 0) 
	{
		LOG_INF("psa_key_derivation_input_bytes failed! (Error: %d)", status);
		return -1;
	}

	/* Store the derived key in the keystore slot pointed by out_key_id */
	status = psa_key_derivation_output_key(&key_attributes, &operation, &output_key_id);
	if (status != 0) 
	{
		LOG_INF("psa_key_derivation_output_key failed! (Error: %d)", status);
		return -1;
	}

	/* Clean up the context */
	status = psa_key_derivation_abort(&operation);
	if (status != 0) 
	{
		LOG_INF("psa_key_derivation_abort failed! (Error: %d)", status);
		return -1;
	}

	return APP_SUCCESS;
}

static int crypto_destroy_input_key(psa_key_id_t *input_key_id)
{
	psa_status_t status;

	/* Destroy the key handle */
	status = psa_destroy_key(input_key_id);
	if (status != PSA_SUCCESS) 
	{
		LOG_INF("psa_destroy_key failed! (Error: %d)", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

static int crypto_destroy_output_key(psa_key_id_t *output_key_id)
{
	psa_status_t status;

	status = psa_destroy_key(output_key_id);
	if (status != PSA_SUCCESS) 
	{
		LOG_INF("psa_destroy_key failed! (Error: %d)", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

crypto_status_t crypto_kdf_hkdf(crypto_kdf_alg_t alg,
					const uint8_t *salt, size_t salt_len,
					const uint8_t *ikm, size_t ikm_len,
					const uint8_t *info, size_t info_len,
					uint8_t *okm, size_t okm_len)
{
	int status;
	psa_key_id_t input_key_id;
	psa_key_id_t output_key_id;

	LOG_INF("Starting HKDF process");

	status = import_input_key(ikm, ikm_len, input_key_id);
	if (status != 0) 
	{
		LOG_INF("Input Key Import Failed");
		return -1;
	}

	status = derive_hkdf(input_key_id, output_key_id, 
						 salt, salt_len,
						 ikm, ikm_len,
						 info, info_len,
						 okm_len);
	if (status != 0) 
	{
		LOG_INF("Key Derivation Process Failed");
		return -1;
	}

	status = export_output_key(okm, okm_len, output_key_id);
	if (status != APP_SUCCESS) 
	{
		LOG_INF("Output Key Export Failed");
		return -1;
	}

	status = crypto_destroy_input_key(input_key_id);
	if (status != 0) 
	{
		LOG_INF("Input Key Destruction Failed");
		return -1;
	}

	status = crypto_destroy_output_key(output_key_id);
	if (status != 0) 
	{
		LOG_INF("Output Key Destruction Failed");
		return -1;
	}

	LOG_INF("HKDF Process Successfull");

	return 0;
}