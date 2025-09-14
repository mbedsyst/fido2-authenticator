#ifndef AUTHENTICATOR_GET_INFO_H
#define AUTHENTICATOR_GET_INFO_H

#include <stdint.h>

int authenticator_get_info_handler(const uint8_t *req, size_t req_len,
                                   uint8_t *resp, size_t *resp_len);

#endif // AUTHENTICATOR_GET_INFO_H
