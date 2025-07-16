#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include "logger.h"
#include "app_ctx.h"
#include "fido_main.h"

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(UART_DEVICE_NODE);
    int ret = log_initialize(dev, LOG_LEVEL_DEBUG);

    if (ret)
    {
        printk("Logger failed (%d)\n", ret);
    }

    log_info("FIDO2 Authenticator Booting");

    app_ctx_init();
    fido_main();

    return 0;
}