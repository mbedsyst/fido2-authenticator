#ifndef AUTHENTICATOR_GET_ASSERTION_H
#define AUTHENTICATOR_GET_ASSERTION_H

#include <stdint.h>

int authenticator_get_assertion_handler(const uint8_t *req, size_t req_len,
                                        uint8_t *resp, size_t *resp_len);

#endif // AUTHENTICATOR_GET_ASSERTION_H
