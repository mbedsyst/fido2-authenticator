#ifndef CORE_STATE_MACHINE_STATE_HANDLERS_H_
#define CORE_STATE_MACHINE_STATE_HANDLERS_H_

#include "app_events.h"


void handle_idle(app_ctx_t *ctx, app_event_t event);
void handle_receiving(app_ctx_t *ctx, app_event_t event);
void handle_reconstructing(app_ctx_t *ctx, app_event_t event);
void handle_processing(app_ctx_t *ctx, app_event_t event);
void handle_deconstructing(app_ctx_t *ctx, app_event_t event);
void handle_responding(app_ctx_t *ctx, app_event_t event);
void handle_error(app_ctx_t *ctx, app_event_t event);

#endif