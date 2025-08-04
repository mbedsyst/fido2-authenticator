#include "error_mapper.h"

uint8_t remap_error_to_ctaphid(app_ctx_t *ctx)
{
    switch(ctx->local_error_domain)
    {
        case ERROR_DOMAIN_CTAP2_PARSER:
            switch(ctx->local_error_code)
            {
                case 0 : break;
                case 1 : break;
                default: break;
            }
            break;

        case ERROR_DOMAIN_CTAP2_PROCESSOR:
        switch(ctx->local_error_code)
            {
                case 0 : break;
                case 1 : break;
                default: break;
            }
            break;

        case ERROR_DOMAIN_CRYPTO:
        switch(ctx->local_error_code)
            {
                case 0 : break;
                case 1 : break;
                default: break;
            }
            break;

        case ERROR_DOMAIN_STORAGE:
        switch(ctx->local_error_code)
            {
                case 0 : break;
                case 1 : break;
                default: break;
            }
            break;

        case ERROR_DOMAIN_TRANSPORT:
            switch(ctx->local_error_code)
            {
                case 0 : break;
                case 1 : break;
                default: break;
            }
            break;

        default:
            break;
    }
}