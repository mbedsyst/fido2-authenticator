#ifndef AUTHENTICATOR_RESET_H
#define AUTHENTICATOR_RESET_H

#include <stdint.h>

int authenticator_reset_handler(const uint8_t *req, size_t req_len,
                                uint8_t *resp, size_t *resp_len);

#endif // AUTHENTICATOR_RESET_H
