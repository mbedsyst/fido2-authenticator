#include "interfaces/crypto/symmetric/cipher.h"
#include "crypto_status.h"

#include <stdio.h>
#include <stdlib.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>

LOG_MODULE_REGISTER(crypto_cipher_interface);

static int import_cipher_encryption_key(const uint8_t *key, size_t key_len, psa_key_id_t *key_id)
{
	psa_status_t status;

	LOG_INF("Importing the Cipher Key");

	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_GCM);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&key_attributes, 128);

	status = psa_import_key(&key_attributes,
							key,
							key_len,
							&key_id);
	if (status != PSA_SUCCESS) 
	{
		LOG_INF("psa_import_key failed! (Error: %d)", status);
		return APP_ERROR;
	}

	/* After the key handle is acquired the attributes are not needed */
	psa_reset_key_attributes(&key_attributes);

	LOG_INF("Cipher Key imported successfully!");

	return 0;
}

static int import_cipher_decryption_key(const uint8_t *key, size_t key_len, psa_key_id_t *key_id)
{
	psa_status_t status;

	LOG_INF("Importing the Cipher Key");

	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_DECRYPT);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_GCM);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&key_attributes, 128);

	status = psa_import_key(&key_attributes,
							key,
							key_len,
							&key_id);
	if (status != PSA_SUCCESS) 
	{
		LOG_INF("psa_import_key failed! (Error: %d)", status);
		return APP_ERROR;
	}

	/* After the key handle is acquired the attributes are not needed */
	psa_reset_key_attributes(&key_attributes);

	LOG_INF("Cipher Key imported successfully!");

	return 0;
}

static int encrypt_aes_gcm(psa_key_id_t * key_id,
					const uint8_t * iv, size_t iv_len,
					const uint8_t * additional_data, size_t additional_data_len,
					const uint8_t * plaintext, size_t plaintext_len,
						  uint8_t * ciphertext, size_t ciphertext_size)
{
	uint32_t output_len;
	psa_status_t status;

	LOG_INF("Encrypting using AES-GCM Mode");

	/* Encrypt the plaintext and create the authentication tag */
	status = psa_aead_encrypt(key_id, PSA_ALG_GCM,
							  iv, iv_len,
							  additional_data, additional_data_len,
							  plaintext, plaintext_len,
							  ciphertext, ciphertext_size,
							  &output_len);

	if (status != 0) 
	{
		LOG_INF("psa_aead_encrypt failed! (Error: %d)", status);
		return -1;
	}

	LOG_INF("Encryption successful!");

	return APP_SUCCESS;
}

static int decrypt_aes_gcm(psa_key_id_t * key_id,
					const uint8_t * iv, size_t iv_len,
					const uint8_t * additional_data, size_t additional_data_len,
					const uint8_t * ciphertext , size_t ciphertext_len,
						  uint8_t * plaintext, size_t plaintext_size)
{
	uint32_t output_len;
	psa_status_t status;

	LOG_INF("Decrypting using AES GCM MODE...");

	/* Decrypt and authenticate the encrypted data */
	status = psa_aead_decrypt(key_id,
								PSA_ALG_GCM,
								iv, iv_len,
								additional_data, additional_data_len,
								ciphertext, ciphertext_len,
								plaintext, plaintext_size,
								&output_len);
	if (status != 0) 
	{
		LOG_INF("psa_aead_decrypt failed! (Error: %d)", status);
		return -1;
	}

	LOG_INF("Decryption and authentication successful!");

	return APP_SUCCESS;
}

static int crypto_destroy_key(psa_key_id_t *key_id)
{
	psa_status_t status;

	/* Destroy the key handle */
	status = psa_destroy_key(key_id);
	if (status != 0) 
	{
		LOG_INF("psa_destroy_key failed! (Error: %d)", status);
		return -1;
	}

	return 0;
}


int crypto_cipher_encrypt(const uint8_t *key, size_t key_len
                          const uint8_t *iv, size_t iv_len,
                          const uint8_t *aad, size_t aad_len,
                          const uint8_t *input, size_t input_len,
                          uint8_t *output, size_t *output_len,
                          uint8_t *tag, size_t tag_len)
{
	int status;
	psa_key_id_t key_id

	LOG_INF("Starting AES-GCM Encryption");

	status = import_cipher_encryption_key(key, key_len, key_id);
	if (status != 0) 
	{
		LOG_INF("Encryption Key Import Failed");
		return -1;
	}

	status = encrypt_aes_gcm(key_id, iv, iv_len, aad, aad_len, input, input_len, output, output_len);
	if (status != 0) 
	{
		LOG_INF("AES GCM Encryption Failed");
		return -1;
	}

	status = crypto_destroy_key(key_id);
	if (status != 0) 
	{
		LOG_INF("Encryption Key Destruction Failed");
		return -1;
	}

	LOG_INF("AES GCM Encryption Successfull");

	return 0;
}

int crypto_cipher_decrypt(const uint8_t *key, size_t key_len,
                          const uint8_t *iv, size_t iv_len,
                          const uint8_t *aad, size_t aad_len,
                          const uint8_t *input, size_t input_len,
                          uint8_t *output, size_t *output_len,
                          const uint8_t *tag, size_t tag_len)
{
	int status;
	psa_key_id_t key_id

	LOG_INF("Starting AES-GCM Decryption");

	status = import_cipher_decryption_key(key, key_len, key_id);
	if (status != 0) 
	{
		LOG_INF("Decryption Key Import Failed");
		return -1;
	}

	status = decrypt_aes_gcm(key_id, iv, iv_len, aad, aad_len, input, input_len, output, output_len);
	if (status != 0) 
	{
		LOG_INF("AES GCM Decryption Failed");
		return -1;
	}

	status = crypto_destroy_key(key_id);
	if (status != 0) 
	{
		LOG_INF("Decryption Key Destruction Failed");
		return -1;
	}

	LOG_INF("AES GCM Decryption Successfull");

	return APP_SUCCESS;
}
