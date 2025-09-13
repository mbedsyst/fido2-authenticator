#include "interfaces/flash/flash_if.h"
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h> // for flash_area APIs
#include <zephyr/kernel.h>
#include <string.h>

#define FLASH_DEV_LABEL DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL

static const struct device *flash_dev = NULL;

/* Optional: partition handle storage */
struct partition_ctx {
    const struct flash_area *fa;
};
typedef struct partition_ctx partition_ctx_t;

flash_status_t flash_if_init(void)
{
    if (flash_dev) {
        return FLASH_OK; /* already init'd */
    }

    flash_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));
    if (!device_is_ready(flash_dev)) {
        return FLASH_ERR_NO_DEVICE;
    }

    return FLASH_OK;
}

flash_status_t flash_if_read(uint32_t offset, void *buf, size_t len)
{
    if (!flash_dev || !buf) return FLASH_ERR_INVALID_ARG;
    int rc = flash_read(flash_dev, (off_t)offset, buf, len);
    return rc == 0 ? FLASH_OK : FLASH_ERR_READ_FAIL;
}

flash_status_t flash_if_write(uint32_t offset, const void *buf, size_t len)
{
    if (!flash_dev || !buf) return FLASH_ERR_INVALID_ARG;

    /* Note: check write alignment if required */
    size_t wbs;
    flash_if_get_write_block_size(&wbs);
    if (wbs > 1 && ((offset % wbs) != 0)) {
        return FLASH_ERR_ALIGN;
    }

    int rc = flash_write(flash_dev, (off_t)offset, buf, len);
    return rc == 0 ? FLASH_OK : FLASH_ERR_WRITE_FAIL;
}

flash_status_t flash_if_erase(uint32_t offset, size_t len)
{
    if (!flash_dev) return FLASH_ERR_INVALID_ARG;

    size_t erase_unit;
    flash_if_get_erase_unit_size(&erase_unit);
    if (erase_unit > 0 && ((offset % erase_unit) != 0 || (len % erase_unit) != 0)) {
        return FLASH_ERR_ALIGN;
    }

    int rc = flash_erase(flash_dev, (off_t)offset, len);
    return rc == 0 ? FLASH_OK : FLASH_ERR_ERASE_FAIL;
}

flash_status_t flash_if_get_size(uint32_t *size_out)
{
    if (!flash_dev || !size_out) return FLASH_ERR_INVALID_ARG;

    /* Many drivers expose size via device tree; try flash_get_page_info_by_offs to infer */
    uint32_t total_size = 0;
    const struct flash_driver_api *api = (const struct flash_driver_api *)flash_dev->api;
    if (api && api->read) {
        /* best-effort: attempt to get via device tree property or flash_area API externally */
        /* For now, try page layout approach: */
        struct flash_pages_info info;
        int rc = flash_get_page_info_by_offs(flash_dev, 0, &info);
        if (rc == 0) {
            /* This returns first page info; not total. For now rely on DT or user pass */
            /* Caller might prefer flash_area_get_size for partitions */
            *size_out = 0; /* not implemented reliably */
            return FLASH_ERR_NOT_IMPLEMENTED;
        }
    }

    return FLASH_ERR_NOT_IMPLEMENTED;
}

flash_status_t flash_if_get_write_block_size(size_t *wb_size_out)
{
    if (!flash_dev || !wb_size_out) return FLASH_ERR_INVALID_ARG;
    *wb_size_out = flash_get_write_block_size(flash_dev);
    return FLASH_OK;
}

flash_status_t flash_if_get_erase_unit_size(size_t *erase_size_out)
{
    if (!flash_dev || !erase_size_out) return FLASH_ERR_INVALID_ARG;

    struct flash_pages_info info;
    int rc = flash_get_page_info_by_offs(flash_dev, 0, &info);
    if (rc == 0) {
        *erase_size_out = info.size;
        return FLASH_OK;
    }
    return FLASH_ERR_NOT_IMPLEMENTED;
}

flash_status_t flash_if_get_erase_value(uint8_t *value_out)
{
    if (!flash_dev || !value_out) return FLASH_ERR_INVALID_ARG;
    *value_out = flash_get_erase_value(flash_dev);
    return FLASH_OK;
}

flash_status_t flash_if_sync(void)
{
    /* Zephyr flash drivers are typically synchronous */
    return FLASH_OK;
}

/* Partition helpers using flash_area (optional) */

flash_status_t flash_if_partition_open(const char *name, flash_partition_handle_t *h_out)
{
    if (!name || !h_out) return FLASH_ERR_INVALID_ARG;
    const struct flash_area *fa;
    int rc = flash_area_open(FLASH_AREA_ID(name), &fa);
    if (rc < 0) {
        /* try name-based search - flash_area_open_by_id or dt node based */
        return FLASH_ERR_NOT_IMPLEMENTED;
    }
    partition_ctx_t *ctx = (partition_ctx_t *)k_malloc(sizeof(partition_ctx_t));
    if (!ctx) return FLASH_ERR_GENERIC;
    ctx->fa = fa;
    *h_out = ctx;
    return FLASH_OK;
}

void flash_if_partition_close(flash_partition_handle_t h)
{
    if (!h) return;
    partition_ctx_t *ctx = (partition_ctx_t *)h;
    flash_area_close(ctx->fa);
    k_free(ctx);
}

flash_status_t flash_if_partition_read(flash_partition_handle_t h, uint32_t part_off, void *buf, size_t len)
{
    if (!h || !buf) return FLASH_ERR_INVALID_ARG;
    partition_ctx_t *ctx = (partition_ctx_t *)h;
    int rc = flash_area_read(ctx->fa, part_off, buf, len);
    return rc == 0 ? FLASH_OK : FLASH_ERR_READ_FAIL;
}

flash_status_t flash_if_partition_write(flash_partition_handle_t h, uint32_t part_off, const void *buf, size_t len)
{
    if (!h || !buf) return FLASH_ERR_INVALID_ARG;
    partition_ctx_t *ctx = (partition_ctx_t *)h;
    int rc = flash_area_write(ctx->fa, part_off, buf, len);
    return rc == 0 ? FLASH_OK : FLASH_ERR_WRITE_FAIL;
}

flash_status_t flash_if_partition_erase(flash_partition_handle_t h, uint32_t part_off, size_t len)
{
    if (!h) return FLASH_ERR_INVALID_ARG;
    partition_ctx_t *ctx = (partition_ctx_t *)h;
    int rc = flash_area_erase(ctx->fa, part_off, len);
    return rc == 0 ? FLASH_OK : FLASH_ERR_ERASE_FAIL;
}

