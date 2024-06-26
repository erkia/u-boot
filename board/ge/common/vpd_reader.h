/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2016 General Electric Company
 */

#include <linux/types.h>

struct vpd_cache;

/*
 * Read VPD from given data, verify content, call callback for each vital
 * product data block.
 *
 * cache: structure used by process block to store VPD information
 * process_block: callback called for each VPD data block
 *
 * Returns Non-zero on error.  Negative numbers encode errno.
 */
int read_i2c_vpd(struct vpd_cache *cache,
		 int (*process_block)(struct vpd_cache *, u8 id, u8 version,
				      u8 type, size_t size, u8 const *data));

/*
 * Read VPD from given data, verify content, call callback for each vital
 * product data block.
 *
 * size: size of the raw VPD data in bytes
 * data: raw VPD data read from device
 * cache: structure used by process block to store VPD information
 * process_block: callback called for each VPD data block
 *
 * Returns Non-zero on error.  Negative numbers encode errno.
 */

int vpd_reader(size_t size, u8 *data, struct vpd_cache *cache,
	       int (*process_block)(struct vpd_cache *, u8 id, u8 version, u8 type,
				    size_t size, u8 const *data));
