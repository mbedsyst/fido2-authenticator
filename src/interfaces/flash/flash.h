#ifndef FLASH_H
#define FLASH_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h> // off_t if you want

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FLASH_OK = 0,
    FLASH_ERR_GENERIC = -1,
    FLASH_ERR_INVALID_ARG = -2,
    FLASH_ERR_NO_DEVICE = -3,
    FLASH_ERR_ALIGN = -4,
    FLASH_ERR_OUT_OF_RANGE = -5,
    FLASH_ERR_ERASE_FAIL = -6,
    FLASH_ERR_WRITE_FAIL = -7,
    FLASH_ERR_READ_FAIL = -8,
    FLASH_ERR_UNSUPPORTED = -9,
    FLASH_ERR_NOT_IMPLEMENTED = -100,
} flash_status_t;

/**
 * Initialize flash interface / driver. Must be called once at startup.
 * Backend should bind to device (device_get_binding) and prepare any internal state.
 */
flash_status_t flash_if_init(void);

/**
 * Read raw bytes from the flash device (absolute offset).
 * @param offset Absolute offset from flash base (or partition base if using partitioned driver).
 * @param buf Buffer to fill, must be non-NULL.
 * @param len Number of bytes to read.
 */
flash_status_t flash_if_read(uint32_t offset, void *buf, size_t len);

/**
 * Write raw bytes to the flash device. Behavior: backend must obey device write granularity.
 * Some backends may require alignment of offset/len to write block size.
 */
flash_status_t flash_if_write(uint32_t offset, const void *buf, size_t len);

/**
 * Erase region at offset for length bytes. Offset and length must be aligned to erase unit.
 */
flash_status_t flash_if_erase(uint32_t offset, size_t len);

/**
 * Returns the total flash size in bytes (for the device this driver represents).
 */
flash_status_t flash_if_get_size(uint32_t *size_out);

/**
 * Returns the minimum write block size (granularity). Useful for alignment.
 */
flash_status_t flash_if_get_write_block_size(size_t *wb_size_out);

/**
 * Returns the erase unit/page size (sector size).
 */
flash_status_t flash_if_get_erase_unit_size(size_t *erase_size_out);

/**
 * Returns the flash default erased value (usually 0xFF).
 */
flash_status_t flash_if_get_erase_value(uint8_t *value_out);

/**
 * Optional: sync, flush caches. On many NOR devices not required.
 */
flash_status_t flash_if_sync(void);

/* Optional helpers if using flash_area partitions: */

/**
 * Open a partition by id or name - backend may ignore if not supported.
 * Returns a handle (opaque) to be used with partition-based read/write/erase.
 */
typedef void *flash_partition_handle_t;
flash_status_t flash_if_partition_open(const char *name, flash_partition_handle_t *h_out);
void flash_if_partition_close(flash_partition_handle_t h);

/**
 * Partition read/write/erase APIs (offset relative to partition start).
 */
flash_status_t flash_if_partition_read(flash_partition_handle_t h, uint32_t part_off, void *buf, size_t len);
flash_status_t flash_if_partition_write(flash_partition_handle_t h, uint32_t part_off, const void *buf, size_t len);
flash_status_t flash_if_partition_erase(flash_partition_handle_t h, uint32_t part_off, size_t len);

#ifdef __cplusplus
}
#endif

#endif // FLASH_H
