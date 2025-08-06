#ifndef CORE_FAST_PATH_FAST_PATH_RESPONDER_H_
#define CORE_FAST_PATH_FAST_PATH_RESPONDER_H_

#include <zephyr/kernel.h>
#include <stdint.h>

void fast_responder_init(app_ctx_t *ctx);
int fast_responder_enqueue(uint32_t cid, uint8_t cmd, uint8_t error_code);

#endif