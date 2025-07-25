#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "app_ctx.h"
#include "fido_main.h"

LOG_MODULE_REGISTER(main);

int main(void)
{
    LOG_INF("Entry to main()");
    
    app_ctx_init();
    fido_main();

    return 0;
}