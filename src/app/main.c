#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


#include "app_ctx.h"
#include "fido_main.h"
#include "crypto_system.h"

LOG_MODULE_REGISTER(main);

int main(void)
{
    LOG_INF("Entry to main()");

    if (crypto_system_init() != CRYPTO_SUCCESS) 
    {
        LOG_ERR("Crypto Interface initialization failed!");
        return -1;
    }
    
    app_ctx_init();
    fido_main();

    return 0;
}