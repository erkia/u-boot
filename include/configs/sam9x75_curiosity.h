/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration settings for the SAM9X75 CURIOSITY board.
 *
 * Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries
 *
 * Author: Manikandan Muralidharan <manikandan.m@microchip.com>
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CFG_SYS_AT91_SLOW_CLOCK	32768
#define CFG_SYS_AT91_MAIN_CLOCK	24000000	/* 24 MHz crystal */

#define CFG_USART_BASE   ATMEL_BASE_DBGU
#define CFG_USART_ID     0 /* ignored in arm */

/* SDRAM */
#define CFG_SYS_SDRAM_BASE		0x20000000
#define CFG_SYS_SDRAM_SIZE		0x10000000      /* 256 megs */

#endif
