// SPDX-License-Identifier:	GPL-2.0+
/*
 * (C) Copyright 2019
 * Rockchip Electronics Co., Ltd
 * Kever Yang<kever.yang@rock-chips.com>
 *
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 *	Aneesh V <aneesh@ti.com>
 */

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)
SECTIONS
{
	. = 0x00000000;

	__image_copy_start = ADDR(.text);
	.text : {
		. = ALIGN(8);
		CPUDIR/start.o (.text*)
		*(.text*)
	}

	.rodata : {
		. = ALIGN(8);
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	}

	.data : {
		. = ALIGN(8);
		*(.data*)
	}

	__u_boot_list : {
		. = ALIGN(8);
		KEEP(*(SORT(__u_boot_list*)));
	}

	. = ALIGN(8);
	__image_copy_end = .;
	_end = .;
	_image_binary_end = .;

	.bss ALIGN(8) : {
		__bss_start = .;
		*(.bss*)
		. = ALIGN(8);
		__bss_end = .;
	}

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
}

#if defined(CONFIG_TPL_MAX_SIZE)
ASSERT(__image_copy_end - __image_copy_start < (CONFIG_TPL_MAX_SIZE), \
	"TPL image too big");
#endif

#if defined(CONFIG_TPL_BSS_MAX_SIZE)
ASSERT(__bss_end - __bss_start < (CONFIG_TPL_BSS_MAX_SIZE), \
	"TPL image BSS too big");
#endif

#if defined(CONFIG_TPL_MAX_FOOTPRINT)
ASSERT(__bss_end - _start < (CONFIG_TPL_MAX_FOOTPRINT), \
	"TPL image plus BSS too big");
#endif
