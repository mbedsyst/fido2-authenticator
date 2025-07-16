#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include "led_controller.h"

#define SLOW_PERIOD_MS  1000
#define FAST_PERIOD_MS  200

struct led_ctx
{
    const struct gpio_dt_spec pin;
    led_state_t current;
    struct k_timer blink_timer;    
};

static struct led_ctx leds[LED_COUNT] =
{
    [LED_STATUS] =
    {
        .pin = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios)
    },
    [LED_ERROR] =
    {
        .pin = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios)
    },
    [LED_IDLE] =
    {
        .pin = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios)
    },
    [LED_OPERATION] =
    {
        .pin = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios)
    },
};

static void blink_cb(struct k_timer *timer);

static int led_init(void)
{
    for(size_t i = 0; i < LED_COUNT; ++i)
    {
        if(!device_is_ready(leds[i].pin.port))
        {
            return -ENODEV;
        }
        gpio_pin_configure_dt(&leds[i].pin, GPIO_OUTPUT_INACTIVE);
        k_timer_init(&leds[i].blink_timer, blink_cb, NULL);
        leds[i].blink_timer.user_data = &leds[i];
        leds[i].current = LED_OFF;
    }
    return 0;
}

static int led_set(led_id_t id, led_state_t state)
{
    if (id >= LED_COUNT)
    {
        return -EINVAL;
    }

    struct led_ctx *ctx = &leds[id];
    ctx->current = state;

    switch(state)
    {
        case LED_ON:
            gpio_pin_set_dt(&ctx->pin, 1);
            k_timer_stop(&ctx->blink_timer);
            break;
        case LED_OFF:
            gpio_pin_set_dt(&ctx->pin, 0);
            k_timer_stop(&ctx->blink_timer);
            break;
        case LED_BLINK_SLOW:
            k_timer_start(&ctx->blink_timer, K_MSEC(SLOW_PERIOD_MS), K_MSEC(SLOW_PERIOD_MS));
            break;
        case LED_BLINK_FAST:
            k_timer_start(&ctx->blink_timer, K_MSEC(FAST_PERIOD_MS), K_MSEC(FAST_PERIOD_MS));
            break;

        default:
            return -EINVAL;
    }

    return 0;
}

static bool led_is_on(led_id_t id)
{
    if(id >= LED_COUNT)
    {
        return false;
    }
    return leds[id].current == LED_ON;
}

static void blink_cb(struct k_timer *timer)
{
    struct led_ctx *ctx = timer->user_data;
    gpio_pin_toggle_dt(&ctx->pin);
}

const led_if_t led_controller_if =
{
    .init   = led_init,
    .set    = led_set,
    .is_on  = led_is_on
};