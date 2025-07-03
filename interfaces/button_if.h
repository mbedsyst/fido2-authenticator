#ifndef INTERFACES_BUTTON_IF_H_
#define INTERFACES_BUTTON_IF_H_

#include <stdbool.h>
#include <stdint.h>

typedef void (*button_long_cb_t)(void *user_data);

typedef struct 
{
    int     (*init)(void);
    bool    (*is_pressed)(void);
    int     (*register_long_press_cb)(button_long_cb_t cb, void *user_data);
} button_if_t;

#endif