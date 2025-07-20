/**
 * @file    ctaphid_dispatcher.c
 * @brief   Handles command dispatching for CTAPHID Commands
 * 
 * This module is responsible for calling the respective Command
 * Dispatcher based on the CTAPHID Command
 * 
 * @author  Zalman Ul Fariz
 * @date    July 2025
 */

#include <stddef.h>
#include <stdint.h>

#include <zephyr/logging/log.h>

#include "ctaphid.h"
#include "ctaphid_dispatcher.h"

LOG_MODULE_REGISTER(ctaphid_dispatcher);

ctaphid_status_t ctaphid_cmd_msg(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the MSG Command Logic
}

ctaphid_status_t ctaphid_cmd_cbor(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the CBOR Command Logic
}

ctaphid_status_t ctaphid_cmd_init(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the INIT Command Logic
}

ctaphid_status_t ctaphid_cmd_ping(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the PING Command Logic
}

ctaphid_status_t ctaphid_cmd_cancel(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the CANCEL Command Logic
}

ctaphid_status_t ctaphid_cmd_error(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the ERROR Command Logic
}

ctaphid_status_t ctaphid_cmd_keepalive(ctaphid_req_session_t *session)
{
    if(session == NULL)
    {
        LOG_ERR("Received NULL Structure");
        return CTAPHID_ERROR_INVALID_INPUT;
    }
    
    // ToDo: Write the KEEPALIVE Command Logic
}

