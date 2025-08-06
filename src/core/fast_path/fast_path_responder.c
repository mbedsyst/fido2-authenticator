#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "app_ctx.h"
#include "ctaphid.h"
#include "fast_path_responder.h"

LOG_MODULE_REGISTER(fast_responder);

#define FAST_RESP_QUEUE_SIZE    6
#define FAST_RESP_STACK_SIZE    1024
#define FAST_RESP_PRIORITY      3

#define ERROR_PACKET_SIZE       8

typedef struct
{
    uint32_t cid;
    uint8_t cmd;
    uint8_t error_code;
} fast_resp_task_t;

static app_ctx_t *g_ctx = NULL;

K_MSGQ_DEFINE(fast_resp_msgq, sizeof(fast_resp_task_t), FAST_RESP_QUEUE_SIZE, 4);
K_THREAD_STACK_DEFINE(fast_resp_task_t, FAST_RESP_STACK_SIZE);
static struct k_thread fast_resp_thread;

static void send_error_response(uint32_t cid, uint8_t cmd, uint8_t error_code)
{
    if (!g_ctx || !g_ctx->transport || !g_ctx->transport->send) 
    {
        LOG_ERR("Transport not initialized for fast responder");
        return;
    }

    uint8_t error_response_buffer[ERROR_PACKET_SIZE] = {0};

    error_response_buffer[CID_POS + 0] = (cid << 24) & 0xFF;
    error_response_buffer[CID_POS + 1] = (cid << 16) & 0xFF;
    error_response_buffer[CID_POS + 2] = (cid << 8) & 0xFF;
    error_response_buffer[CID_POS + 3] = (cid) & 0xFF;
    error_response_buffer[INIT_CMD_POS] = CTAPHID_ERROR;
    error_response_buffer[INIT_BCNTH_POS] = 0x00;
    error_response_buffer[INIT_BCNTL_POS] = 0x01;
    error_response_buffer[INIT_DATA_POS] = error_code;

    g_ctx->transport->send(error_response_buffer, sizeof(error_response_buffer));
}
static void fast_responder_thread(void *a, void *b, void *c)
{
    fast_resp_task_t task;

    while(1)
    {
        int ret = k_msgq_get(&fast_resp_msgq, &task, K_FOREVER);
        if(ret == 0)
        {
            LOG_DBG("Fast Responder: CID 0x%08x, CMD 0x%02x, ERR 0x%02x", task.cid, task.cmd, task.error_code);
            send_error_response(task.cid, task.cmd, task.error_code);
        }
    }
}

void fast_responder_init(app_ctx_t *ctx)
{
    g_ctx = ctx;

    k_thread_create(&fast_resp_thread,
                    fast_resp_stack,
                    K_THREAD_STACK_SIZEOF(fast_resp_stack),
                    fast_responder_thread,
                    NULL, NULL, NULL,
                    FAST_RESP_PRIORITY, 0, K_NO_WAIT);

    k_thread_name_set(&fast_resp_thread, "fast_responder");
    LOG_INF("Fast Responder Initialized");
}

int fast_responder_enqueue(uint32_t cid, uint8_t cmd, uint8_t error_code)
{
    fast_resp_task_t task =
    {
        .cid        = cid,
        .cmd        = cmd,
        .error_code = error_code
    };

    int ret = k_msgq_put(&fast_resp_msgq, &task, K_NO_WAIT);
    if(ret != 0)
    {
        LOG_WRN("Fast Responder Queue full - error dropped");
    }
    return ret;
}