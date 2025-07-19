#ifndef INTERFACES_LED_IF_H_
#define INTERFACES_LED_IF_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    LED_STATUS,
    LED_ERROR,
    LED_IDLE,
    LED_OPERATION,
    LED_COUNT
} led_id_t;

typedef enum
{
    LED_OFF  = 0,
    LED_ON,
    LED_BLINK_SLOW,
    LED_BLINK_FAST
} led_state_t;

typedef struct
{
    int     (*init)(void);
    int     (*set)(led_id_t id, led_state_t state);
    bool    (*is_on)(led_id_t id);
} led_if_t;


#endif