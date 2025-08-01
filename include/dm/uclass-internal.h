/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2013 Google, Inc
 *
 * (C) Copyright 2012
 * Pavel Herrmann <morpheus.ibis@gmail.com>
 */

#ifndef _DM_UCLASS_INTERNAL_H
#define _DM_UCLASS_INTERNAL_H

#include <dm/ofnode.h>

/*
 * These next two macros DM_UCLASS_INST() and DM_UCLASS_REF() are only allowed
 * in code generated by dtoc, because the ordering is important and if other
 * instances creep in then they may mess up the ordering expected by dtoc.
 *
 * It is OK to use them with 'extern' though, since that does not actually
 * add a new record to the linker_list.
 */

/**
 * DM_UCLASS_INST() - Declare a uclass ready for run-time use
 *
 * This adds an actual struct uclass to a list which is found by driver model
 * on start-up.
 *
 * For example:
 *
 * DM_UCLASS_INST(clk) = {
 *	.uc_drv		= DM_UCLASS_DRIVER_REF(clk),
 * ...
 * };
 *
 * @_name: Name of the uclass. This must be a valid C identifier, used by the
 *	linker_list.
 */
#define DM_UCLASS_INST(_name)						\
	ll_entry_declare(struct uclass, _name, uclass)

/**
 * DM_UCLASS_REF() - Get a reference to a uclass
 *
 * This is useful for referencing a uclass at build time. Before this is used,
 * an extern DM_UCLASS_INST() must have been declared.
 *
 * For example:
 *
 * extern DM_UCLASS_INST(clk);
 *
 * struct uclass *ucs[] = {
 *	DM_UCLASS_REF(clk),
 * }
 *
 * @_name: Name of the uclass. This must be a valid C identifier, used by the
 *	linker_list
 * Return: struct uclass * for the device
 */
#define DM_UCLASS_REF(_name)						\
	ll_entry_ref(struct uclass, _name, uclass)

/**
 * uclass_set_priv() - Set the private data for a uclass
 *
 * This is normally handled by driver model, which automatically allocates
 * private data when an 'auto' size if provided by the uclass driver.
 *
 * Use this function to override normal operation for special situations, such
 * as needing to allocate a variable amount of data.
 *
 * If OF_PLATDATA_RT is enabled, this function cannot be used out of core driver
 * model code, since the pointer must be within the gd->dm_priv_base region.
 *
 * @uc		Uclass to update
 * @priv	New private-data pointer
 */
void uclass_set_priv(struct uclass *uc, void *priv);

/**
 * uclass_find_next_free_seq() - Get the next free sequence number
 *
 * This returns the next free sequence number. This is useful only if
 * OF_CONTROL is not used. The next free sequence number is simply the
 * maximum sequence number used by all devices in the uclass + 1. The value
 * returned is always greater than the largest alias, if DM_SEQ_ALIAS is enabled
 * and the uclass has the DM_UC_FLAG_SEQ_ALIAS flag.
 *
 * This allows assigning the sequence number in the binding order.
 *
 * @uc:		uclass to check
 * Return:	The next free sequence number
 */
int uclass_find_next_free_seq(struct uclass *uc);

/**
 * uclass_get_device_tail() - handle the end of a get_device call
 *
 * This handles returning an error or probing a device as needed.
 *
 * @dev: Device that needs to be probed
 * @ret: Error to return. If non-zero then the device is not probed
 * @devp: Returns the value of 'dev' if there is no error
 * Return: ret, if non-zero, else the result of the device_probe() call
 */
int uclass_get_device_tail(struct udevice *dev, int ret, struct udevice **devp);

/**
 * dev_get_uclass_index() - Get uclass and index of device
 * @dev:	- in - Device that we want the uclass/index of
 * @ucp:	- out - A pointer to the uclass the device belongs to
 *
 * The device is not prepared for use - this is an internal function.
 *
 * Return: the index of the device in the uclass list or -ENODEV if not found.
 */
int dev_get_uclass_index(struct udevice *dev, struct uclass **ucp);

/**
 * uclass_find_device() - Return n-th child of uclass
 * @id:		Id number of the uclass
 * @index:	Position of the child in uclass's list
 * @devp:	Returns pointer to device, or NULL on error
 *
 * The device is not prepared for use - this is an internal function.
 * The function uclass_get_device_tail() can be used to probe the device.
 *
 * Return: the uclass pointer of a child at the given index or
 * return NULL on error.
 */
int uclass_find_device(enum uclass_id id, int index, struct udevice **devp);

/**
 * uclass_find_first_device() - Return the first device in a uclass
 * @id:		Id number of the uclass
 * @devp:	Returns pointer to device, or NULL on error
 *
 * The device is not prepared for use - this is an internal function.
 * The function uclass_get_device_tail() can be used to probe the device.
 *
 * Return: 0 if OK (found or not found), -ve on error
 */
int uclass_find_first_device(enum uclass_id id, struct udevice **devp);

/**
 * uclass_find_next_device() - Return the next device in a uclass
 * @devp: On entry, pointer to device to lookup. On exit, returns pointer
 * to the next device in the same uclass, or NULL if none
 *
 * The device is not prepared for use - this is an internal function.
 * The function uclass_get_device_tail() can be used to probe the device.
 */
void uclass_find_next_device(struct udevice **devp);

/**
 * uclass_find_device_by_namelen() - Find uclass device based on ID and name
 *
 * This searches for a device with the exactly given name.
 *
 * The device is NOT probed, it is merely returned.
 *
 * @id: ID to look up
 * @name: name of a device to find
 * @len: Length of @name (the uclass driver name must have the same length)
 * @devp: Returns pointer to device (the first one with the name)
 * Return: 0 if OK, -ve on error
 */
int uclass_find_device_by_namelen(enum uclass_id id, const char *name, int len,
				  struct udevice **devp);

/**
 * uclass_find_device_by_name() - Find uclass device based on ID and name
 *
 * This searches for a device with the exactly given name.
 *
 * The device is NOT probed, it is merely returned.
 *
 * @id: ID to look up
 * @name: name of a device to find
 * @devp: Returns pointer to device (the first one with the name)
 * Return: 0 if OK, -ve on error
 */
int uclass_find_device_by_name(enum uclass_id id, const char *name,
			       struct udevice **devp);

/**
 * uclass_find_device_by_seq() - Find uclass device based on ID and sequence
 *
 * This searches for a device with the given seq.
 *
 * The device is NOT probed, it is merely returned.
 *
 * @id: ID to look up
 * @seq: Sequence number to find (0=first)
 * @devp: Returns pointer to device (there is only one per for each seq)
 * Return: 0 if OK, -ENODEV if not found
 */
int uclass_find_device_by_seq(enum uclass_id id, int seq,
			      struct udevice **devp);

/**
 * uclass_find_device_by_of_offset() - Find a uclass device by device tree node
 *
 * This searches the devices in the uclass for one attached to the given
 * device tree node.
 *
 * The device is NOT probed, it is merely returned.
 *
 * @id: ID to look up
 * @node: Device tree offset to search for (if -ve then -ENODEV is returned)
 * @devp: Returns pointer to device (there is only one for each node)
 * Return: 0 if OK, -ve on error
 */
int uclass_find_device_by_of_offset(enum uclass_id id, int node,
				    struct udevice **devp);

/**
 * uclass_find_device_by_of_node() - Find a uclass device by device tree node
 *
 * This searches the devices in the uclass for one attached to the given
 * device tree node.
 *
 * The device is NOT probed, it is merely returned.
 *
 * @id: ID to look up
 * @node: Device tree offset to search for (if NULL then -ENODEV is returned)
 * @devp: Returns pointer to device (there is only one for each node)
 * Return: 0 if OK, -ve on error
 */
int uclass_find_device_by_ofnode(enum uclass_id id, ofnode node,
				 struct udevice **devp);

/**
 * uclass_find_device_by_phandle() - Find a uclass device by phandle
 *
 * This searches the devices in the uclass for one with the given phandle.
 *
 * The device is NOT probed, it is merely returned.
 *
 * @id: ID to look up
 * @parent: Parent device containing the phandle pointer
 * @name: Name of property in the parent device node
 * @devp: Returns pointer to device (there is only one for each node)
 * Return: 0 if OK, -ENOENT if there is no @name present in the node, other
 *	-ve on error
 */
int uclass_find_device_by_phandle(enum uclass_id id, struct udevice *parent,
				  const char *name, struct udevice **devp);

/**
 * uclass_bind_device() - Associate device with a uclass
 *
 * Connect the device into uclass's list of devices.
 *
 * @dev:	Pointer to the device
 * Return: 0 on success, -ve on error
 */
int uclass_bind_device(struct udevice *dev);

#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
/**
 * uclass_pre_unbind_device() - Prepare to deassociate device with a uclass
 *
 * Call any handled needed before uclass_unbind_device() is called
 *
 * @dev:	Pointer to the device
 * Return: 0 on success, -ve on error
 */
int uclass_pre_unbind_device(struct udevice *dev);

/**
 * uclass_unbind_device() - Deassociate device with a uclass
 *
 * Disconnect the device from uclass's list of devices.
 *
 * @dev:	Pointer to the device
 * Return: 0 on success, -ve on error
 */
int uclass_unbind_device(struct udevice *dev);

#else
static inline int uclass_pre_unbind_device(struct udevice *dev) { return 0; }
static inline int uclass_unbind_device(struct udevice *dev) { return 0; }
#endif

/**
 * uclass_pre_probe_device() - Deal with a device that is about to be probed
 *
 * Perform any pre-processing that is needed by the uclass before it can be
 * probed. This includes the uclass' pre-probe() method and the parent
 * uclass' child_pre_probe() method.
 *
 * @dev:	Pointer to the device
 * Return: 0 on success, -ve on error
 */
int uclass_pre_probe_device(struct udevice *dev);

/**
 * uclass_post_probe_device() - Deal with a device that has just been probed
 *
 * Perform any post-processing of a probed device that is needed by the
 * uclass.
 *
 * @dev:	Pointer to the device
 * Return: 0 on success, -ve on error
 */
int uclass_post_probe_device(struct udevice *dev);

/**
 * uclass_pre_remove_device() - Handle a device which is about to be removed
 *
 * Perform any pre-processing of a device that is about to be removed.
 *
 * @dev:	Pointer to the device
 * Return: 0 on success, -ve on error
 */
#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
int uclass_pre_remove_device(struct udevice *dev);
#else
static inline int uclass_pre_remove_device(struct udevice *dev) { return 0; }
#endif

/**
 * uclass_get_count() - Get the number of uclasses
 *
 * Returns the number of uclasses instantiated in driver model
 */
int uclass_get_count(void);

/**
 * uclass_find() - Find uclass by its id
 *
 * @id:		Id to serach for
 * Return: pointer to uclass, or NULL if not found
 */
struct uclass *uclass_find(enum uclass_id key);

/**
 * uclass_destroy() - Destroy a uclass
 *
 * Destroy a uclass and all its devices
 *
 * @uc: uclass to destroy
 * Return: 0 on success, -ve on error
 */
int uclass_destroy(struct uclass *uc);

#endif
