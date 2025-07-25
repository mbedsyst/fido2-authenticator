#include <string.h>
 
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/class/usb_hid.h>

#include "usb_transport.h"

#define REPORT_SIZE 64

LOG_MODULE_REGISTER(usb_transport);

static const struct device *hid_dev;
static transport_rx_cb_t upper_rx_cb = NULL;
static enum usb_dc_status_code usb_status;
static const uint8_t hid_report_desc[] = 
{
    0x06, 0xD0, 0xF1,       // Usage Page (F1D0)
    0x09, 0x01,             // Usage (CTAPHID)
    0xA1, 0x01,             // Collection (Application)

        0x09, 0x20,         // Usage (Input Report)
        0x15, 0x00,         // Logical Minimum (0)
        0x26, 0xFF, 0x00,   // Logical Maximum (255)
        0x75, 0x08,         // Report Size (8)
        0x95, REPORT_SIZE,  // Report Count
        0x81, 0x02,         // Input (Data,Var,Abs)

        0x09, 0x21,         // Usage (Output Report)
        0x15, 0x00,
        0x26, 0xFF, 0x00,
        0x75, 0x08,
        0x95, REPORT_SIZE,
        0x91, 0x02,         // Output (Data,Var,Abs)

    0xC0                    // End Collection
};

static void int_out_ready_cb(const struct device *dev)
{
    uint8_t buf[REPORT_SIZE];
    uint32_t read_len;

    if (hid_int_ep_read(dev, buf, sizeof(buf), &read_len) == 0) 
    {
        LOG_DBG("Received %d bytes of data as OUT report", read_len);

        if (upper_rx_cb && read_len == REPORT_SIZE) 
        {
            upper_rx_cb(buf, read_len);
        }
    }

    LOG_DBG("Executed the Interrupt IN Callback");
}

static void int_in_ready_cb(const struct device *dev)
{
    LOG_DBG("Executed the Interrupt OUT Callback");
}
 
static const struct hid_ops ops = 
{
    .int_out_ready  = int_out_ready_cb,
    .int_in_ready   = int_in_ready_cb,
};

static inline void status_cb(enum usb_dc_status_code status, const uint8_t *param)
{
    LOG_DBG("Executed the Status Callback");
    usb_status = status;
}

int usb_transport_init(transport_rx_cb_t rx_callback)
{
    upper_rx_cb = rx_callback;
    int ret;

    hid_dev = device_get_binding("HID_0");
    if (hid_dev == NULL) 
    {
        LOG_ERR("Cannot get USB HID Device reference");
        return -ENODEV;
    }

    usb_hid_register_device(hid_dev, hid_report_desc, ARRAY_SIZE(hid_report_desc), &ops);
    LOG_DBG("Registered USB HID Device");

    ret = usb_hid_init(hid_dev);
    if(ret != 0)
    {
        LOG_ERR("Failed to Initialize USB HID Class support");
        return -ENODEV;
    }

    ret = usb_enable(status_cb);
    if (ret != 0) 
    {
        LOG_ERR("Failed to Enable USB Hardware");
        return -ENODEV;
    }

    LOG_DBG("Initialized USB Transport Interface");

    return 0;
}

int usb_transport_send(const uint8_t *data, size_t len)
{
    int ret = 0;

    if (!device_is_ready(hid_dev)) 
    {
        LOG_ERR("USB Device Not Ready");
        return -ENODEV;
    }

    if (len < 0) 
    {
        LOG_ERR("Data Length Invalid");
        return -EINVAL;
    }

    ret = hid_int_ep_write(hid_dev, data, REPORT_SIZE, NULL);
    if(ret != 0)
    {
        LOG_ERR("Failed to write to Endpoint");
        return ret;
    }

    LOG_DBG("Writing %d bytes to IN Endpoint", len);

    return 0;
}

const transport_if_t usb_transport_if = 
{
    .init = usb_transport_init,
    .send = usb_transport_send
};