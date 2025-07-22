/**
 * @file    ctaphid_dispatcher.h
 * @brief   Header for CTAPHID Dispatcher
 * 
 * This header includes the Public function header for
 * the CTAPHID Message Dispatcher
 */

#ifndef CORE_CTAPHID_HANDLER_H_
#define CORE_CTAPHID_HANDLER_H_

#include "app_ctx.h"
#include "ctaphid.h"

ctaphid_status_t ctaphid_cmd_msg(app_ctx_t *ctx);
ctaphid_status_t ctaphid_cmd_cbor(app_ctx_t *ctx);
ctaphid_status_t ctaphid_cmd_init(app_ctx_t *ctx);
ctaphid_status_t ctaphid_cmd_ping(app_ctx_t *ctx);
ctaphid_status_t ctaphid_cmd_cancel(app_ctx_t *ctx);
ctaphid_status_t ctaphid_cmd_error(app_ctx_t *ctx);
ctaphid_status_t ctaphid_cmd_keepalive(app_ctx_t *ctx);

#endif