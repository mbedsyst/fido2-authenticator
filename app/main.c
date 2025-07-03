#include <zephyr/kernel.h>

#include "app_ctx.h"
#include "fido_main.h"

int main(void)
{
    printk("FIDO2 Authenticator Booting\n");

    app_ctx_init();
    fido_main();

    return 0;
}