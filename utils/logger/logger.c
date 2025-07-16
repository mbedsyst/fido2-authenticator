#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "logger.h"

#define COLOR_DEBUG "\033[0;36m"
#define COLOR_INFO  "\033[0;32m"
#define COLOR_WARN  "\033[0;33m"
#define COLOR_ERROR "\033[0;31m"
#define COLOR_RESET "\033[0m"

#define LOG_BUFFER_SIZE 256
#define MSG_BUFFER_SIZE 320 

static const struct device *uart_dev = NULL;
static LogLevel current_level = LOG_LEVEL_DEBUG;

int log_initialize(const struct device *dev, LogLevel level)
{
    if (!device_is_ready(dev)) 
    {
        printk("UART device not ready\n");
        return -ENODEV;
    }

    uart_dev       = dev;
    current_level  = level;
    return 0;
}

static void printlog(const char *buf)
{
    if (!uart_dev) 
    {
        printk("%s", buf);
        return;
    }

    for (const char *p = buf; *p; ++p) 
    {
        uart_poll_out(uart_dev, *p);
    }
}

static void log_generic(LogLevel level, const char *prefix, const char *color, const char *fmt, va_list args)
{
    if (level < current_level) 
    {
        return;
    }

    char msg_buf[LOG_BUFFER_SIZE];
    char final_buf[MSG_BUFFER_SIZE];

    vsnprintf(msg_buf, sizeof msg_buf, fmt, args);

    const char *pad = (strlen(prefix) < 5) ? " " : "";

    snprintf(final_buf, sizeof final_buf, "%s[%s]%s %s%s\r\n", color, prefix, pad, msg_buf, COLOR_RESET);

    printlog(final_buf);
}

void log_debug(const char *fmt, ...)
{
    va_list ap; va_start(ap, fmt);
    log_generic(LOG_LEVEL_DEBUG, "DEBUG", COLOR_DEBUG, fmt, ap);
    va_end(ap);
}

void log_info(const char *fmt, ...)
{
    va_list ap; va_start(ap, fmt);
    log_generic(LOG_LEVEL_INFO,  "INFO",  COLOR_INFO,  fmt, ap);
    va_end(ap);
}

void log_warn(const char *fmt, ...)
{
    va_list ap; va_start(ap, fmt);
    log_generic(LOG_LEVEL_WARN,  "WARN",  COLOR_WARN,  fmt, ap);
    va_end(ap);
}

void log_error(const char *fmt, ...)
{
    va_list ap; va_start(ap, fmt);
    log_generic(LOG_LEVEL_ERROR, "ERROR", COLOR_ERROR, fmt, ap);
    va_end(ap);
}
