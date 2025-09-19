#ifndef CRYPTO_SYSTEM_H_
#define CRYPTO_SYSTEM_H_

#include <psa/crypto.h>

#define CRYPTO_SUCCESS   0
#define CRYPTO_ERROR    -1

int crypto_system_init(void);

#endif
