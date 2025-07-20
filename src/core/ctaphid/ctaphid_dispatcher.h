/**
 * @file    ctaphid_dispatcher.h
 * @brief   Header for CTAPHID Dispatcher
 * 
 * This header includes the Public function header for
 * the CTAPHID Message Dispatcher
 */

#ifndef CORE_CTAPHID_DISPATCHER_H_
#define CORE_CTAPHID_DISPATCHER_H_

ctaphid_status_t ctaphid_cmd_msg(ctaphid_req_session_t *session);
ctaphid_status_t ctaphid_cmd_cbor(ctaphid_req_session_t *session);
ctaphid_status_t ctaphid_cmd_init(ctaphid_req_session_t *session);
ctaphid_status_t ctaphid_cmd_ping(ctaphid_req_session_t *session);
ctaphid_status_t ctaphid_cmd_cancel(ctaphid_req_session_t *session);
ctaphid_status_t ctaphid_cmd_error(ctaphid_req_session_t *session);
ctaphid_status_t ctaphid_cmd_keepalive(ctaphid_req_session_t *session);

#endif