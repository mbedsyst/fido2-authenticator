#ifndef UTILS_LOGGER_LOGGER_H_
#define UTILS_LOGGER_LOGGER_H_

typedef enum
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

int  log_initialize(const struct device *dev, LogLevel level);
void log_debug(const char *fmt, ...);
void log_info (const char *fmt, ...);
void log_warn (const char *fmt, ...);
void log_error(const char *fmt, ...);

#endif