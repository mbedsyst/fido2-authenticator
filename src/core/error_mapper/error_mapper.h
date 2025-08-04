#ifndef CORE_ERROR_MAPPER_ERROR_MAPPER_H_
#define CORE_ERROR_MAPPER_ERROR_MAPPER_H_

#include <stdint.h>
#include "app_ctx.h"
#include "error_domains.h"

uint8_t remap_error_to_ctaphid(app_ctx_t *ctx);

#endif