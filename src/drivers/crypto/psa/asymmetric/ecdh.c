#include "interfaces/crypto/asymmetric/ecdh.h"
#include "interfaces/crypto/crypto_common.h"
#include "crypto_status.h"


#include <stdio.h>
#include <stdlib.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>

LOG_MODULE_REGISTER(crypto_ecdh_interface);


crypto_status_t crypto_ecdh_derive(uint32_t priv_key_id,
                       const uint8_t *peer_pub, size_t peer_pub_len,
                       crypto_ecdh_alg_t alg,
                       uint8_t *shared, size_t *shared_len)
{
    (void)priv_key_id;
    (void)peer_pub;
    (void)peer_pub_len;
    (void)alg;
    (void)shared;
    (void)shared_len;
    return CRYPTO_ERR_NOT_IMPLEMENTED;
}
