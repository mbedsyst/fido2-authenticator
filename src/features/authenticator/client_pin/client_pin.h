#ifndef AUTHENTICATOR_CLIENT_PIN_H
#define AUTHENTICATOR_CLIENT_PIN_H

#include <stdint.h>

int authenticator_client_pin_handler(const uint8_t *req, size_t req_len,
                                     uint8_t *resp, size_t *resp_len);

#endif // AUTHENTICATOR_CLIENT_PIN_H
