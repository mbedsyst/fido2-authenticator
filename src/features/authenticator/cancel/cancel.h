#ifndef AUTHENTICATOR_CANCEL_H
#define AUTHENTICATOR_CANCEL_H

#include <stdint.h>

int authenticator_cancel_handler(const uint8_t *req, size_t req_len,
                                 uint8_t *resp, size_t *resp_len);

#endif // AUTHENTICATOR_CANCEL_H
