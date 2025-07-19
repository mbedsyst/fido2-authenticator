#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/logging/log.h>

#include "button_controller.h"

#define LONG_PRESS_MS   5000u
#define DEBOUNCE_MS     30u

LOG_MODULE_REGISTER(button_controller);

static const struct gpio_dt_spec button_pin = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

struct button_ctx
{
    atomic_t pressed;
    struct gpio_callback gpio_cb;
    struct k_timer long_timer;
    button_long_cb_t long_cb;
    void *cb_user;
};

static struct button_ctx ctx;

static void button_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
static void long_timer_cb(struct k_timer *t);

static int button_init(void)
{
    if(!device_is_ready(button_pin.port))
    {
        return -ENODEV;
    }
    
    int ret = gpio_pin_configure_dt(&button_pin, GPIO_INPUT | GPIO_PULL_UP);
    if(ret)
    {
        LOG_ERR("Failed to Configure GPIO Pin");
        return ret;
    }

    ret = gpio_pin_interrupt_configure_dt(&button_pin, GPIO_INT_EDGE_BOTH);
    if(ret)
    {
        LOG_ERR("Failed to Configure GPIO Pin Interrupt");
        return ret;
    }

    gpio_init_callback(&ctx.gpio_cb, button_isr, BIT(button_pin.pin));
    gpio_add_callback(button_pin.port, &ctx.gpio_cb);

    k_timer_init(&ctx.long_timer, long_timer_cb, NULL);
    ctx.long_timer.user_data = &ctx;

    atomic_clear(&ctx.pressed);
    ctx.long_cb = NULL;
    ctx.cb_user = NULL;

    LOG_DBG("Initialized User Button");

    return 0;
}

static bool button_is_pressed(void)
{
    LOG_DBG("Checking if User Button is Pressed");
    return atomic_get(&ctx.pressed);
}

static int button_register_long_cb(button_long_cb_t cb, void *user)
{
    ctx.long_cb = cb;
    ctx.cb_user = user;
    return 0;
}

static void button_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    static int64_t last_change = 0;
    int64_t now = k_uptime_get();

    if((now - last_change) < DEBOUNCE_MS)
    {
        return;
    }

    last_change = now;

    bool pressed_now = gpio_pin_get_dt(&button_pin) == 0;

    if(pressed_now)
    {
        atomic_set(&ctx.pressed, 1);
        k_timer_start(&ctx.long_timer, K_MSEC(LONG_PRESS_MS), K_NO_WAIT);
    }
    else
    {
        atomic_clear(&ctx.pressed);
        k_timer_stop(&ctx.long_timer);
    }
}

static void long_timer_cb(struct k_timer *t)
{
    if(ctx.long_cb)
    {
        ctx.long_cb(ctx.cb_user);
    }
    else
    {
        sys_reboot(SYS_REBOOT_COLD);
    }
}

const button_if_t button_controller_if =
{
    .init                   = button_init,
    .is_pressed             = button_is_pressed,
    .register_long_press_cb = button_register_long_cb
};

