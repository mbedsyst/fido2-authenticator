#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#include <string.h>

#include "logger.h"

#define UART_NODE DT_CHOSEN(zephyr_shell_uart)

int main(void)
{
    const struct device *uart0 = DEVICE_DT_GET(UART_NODE);

    if (log_init(uart0, LOG_LEVEL_DEBUG) != 0) 
	{
        printk("Logger failed to init\n");
        return 0;
    }

	while(1)
	{
		log_debug("Debug value: %d", 42);
		log_info("System up, built for nRF52840-DK");
		log_warn("Warning, this could breach constant time rule.");
		log_error("Something went wrong!");
		k_msleep(1000);
	}

	return 0;
}
