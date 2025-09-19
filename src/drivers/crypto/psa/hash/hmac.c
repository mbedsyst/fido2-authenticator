#include "interfaces/crypto/hash/hmac.h"
#include "crypto_status.h"

#include <stdio.h>
#include <stdlib.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>
 
LOG_MODULE_REGISTER(crypto_hmac_interface);

static int import_signing_key(psa_key_id_t *key_id, const uint8_t *key, size_t key_len)
{
    psa_status_t status;

    LOG_INF("Importing Application-provided HMAC Key");

    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
    psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
    psa_set_key_algorithm(&key_attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_HMAC);
    psa_set_key_bits(&key_attributes, key_len * 8);

    status = psa_import_key(&key_attributes,
                            key,
                            key_len,
                            &key_id);
    
    psa_reset_key_attributes(&key_attributes);

    if (status != PSA_SUCCESS) 
    {
        LOG_ERR("psa_import_key failed! (Error: %d)", status);
        return -1;
    }

    LOG_INF("HMAC Key imported successfully!");

    return 0;
}

static int import_verification_key(psa_key_id_t *key_id, const uint8_t *key, size_t key_len)
{
    psa_status_t status;

    LOG_INF("Importing Application-provided HMAC Key");

    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

    psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
    psa_set_key_algorithm(&key_attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_HMAC);
    psa_set_key_bits(&key_attributes, key_len * 8);

    status = psa_import_key(&key_attributes,
                            key,
                            key_len,
                            &key_id);

    psa_reset_key_attributes(&key_attributes);

    if (status != PSA_SUCCESS) 
    {
        LOG_ERR("psa_import_key failed! (Error: %d)", status);
        return APP_ERROR;
    }

    LOG_INF("HMAC Key imported successfully!");
   
    return APP_SUCCESS;
}

static int hmac_sign(psa_key_id_t *key_id, const uint8_t *input, size_t input_len, uint8_t *mac, size_t *mac_len)
{
    uint32_t olen;
    psa_status_t status;
    psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;

    LOG_INF("Signing using Imported HMAC Key");

    status = psa_mac_sign_setup(&operation, key_id, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    if (status != PSA_SUCCESS) 
    {
        LOG_ERR("psa_mac_sign_setup failed! (Error: %d)", status);
        return -1;
    }

    status = psa_mac_update(&operation, input, input_len);
    if (status != PSA_SUCCESS) 
    {
        LOG_ERR("psa_mac_update failed! (Error: %d)", status);
        return -1;
    }

    status = psa_mac_sign_finish(&operation, mac, mac_len, &olen);
    if (status != PSA_SUCCESS) 
    {
        LOG_ERR("psa_mac_sign_finish failed! (Error: %d)", status);
        return -1;
    }

    LOG_INF("Signing successful!");

    return 0;
}

static int hmac_verify(psa_key_id_t *key_id, const uint8_t *input, size_t input_len, uint8_t *mac, size_t *mac_len)
{
    psa_status_t status;
    psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;

    LOG_INF("Verifying using Imported HMAC Key");

    status = psa_mac_verify_setup(&operation, key_id, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    if (status != PSA_SUCCESS) 
    {
        LOG_ERR("psa_mac_verify_setup failed! (Error: %d)", status);
        return APP_ERROR;
    }

    status = psa_mac_update(&operation, m_plain_text, sizeof(m_plain_text));
    if (status != PSA_SUCCESS) 
    {
        LOG_ERR("psa_mac_update failed! (Error: %d)", status);
        return APP_ERROR;
    }

    status = psa_mac_verify_finish(&operation, hmac, sizeof(hmac));
    if (status != PSA_SUCCESS) 
    {
        LOG_ERR("psa_mac_verify_finish failed! (Error: %d)", status);
        return APP_ERROR;
    }

    LOG_INF("HMAC verified successfully!");
    return APP_SUCCESS;
}

static int crypto_finish(psa_key_id_t *key_id)
{
    psa_status_t status = psa_destroy_key(key_id);
    if (status != PSA_SUCCESS) 
    {
        LOG_ERR("psa_destroy_key failed! (Error: %d)", status);
        return APP_ERROR;
    }
    return APP_SUCCESS;
}

int crypto_hmac_sign(crypto_hmac_alg_t alg,
                     const uint8_t *key, 
                     size_t key_len,
                     const uint8_t *input, 
                     size_t input_len,
                     uint8_t *mac, 
                     size_t *mac_len)
{
    int status;
    psa_key_id_t key_id;

    LOG_INF("Starting HMAC Signing");

    status = import_signing_key(key_id);
    if (status != 0) 
    {
        LOG_ERR("HMAC Key Import Failed");
        return -1;
    }

    status = hmac_sign(key_id, input, input_len, mac, mac_len);
    if (status != 0) 
    {
        LOG_ERR("HMAC Signing Failed");
        return -1;
    }

    status = crypto_finish(key_id);
    if (status != 0) 
    {
        LOG_ERR("HMAC Key Destruction Failed");
        return -1;
    }

    LOG_INF("HMAC Signing Successfull");

    return 0;
}

int crypto_hmac_verify(crypto_hmac_alg_t alg,
                       const uint8_t *key, 
                       size_t key_len,
                       const uint8_t *input, 
                       size_t input_len,
                       uint8_t *mac, 
                       size_t *mac_len)
{
    int status;
    psa_key_id_t key_id;

    LOG_INF("Starting HMAC Verification");

    status = import_verification_key(key_id);
    if (status != 0) 
    {
        LOG_ERR("HMAC Key Import Failed");
        return -1;
    }

    status = hmac_verify(key_id, input, input_len, mac, mac_len);
    if (status != 0) 
    {
        LOG_ERR("HMAC Verification Failed");
        return -1;
    }

    status = crypto_finish(key_id);
    if (status != 0) 
    {
        LOG_ERR("HMAC Key Destruction Failed");
        return -1;
    }

    LOG_INF("HMAC Verification Successfull");
    return 0;
}