// SPDX-License-Identifier: GPL-2.0+
/*
 * drivers/usb/gadget/dwc2_udc_otg.c
 * Designware DWC2 on-chip full/high speed USB OTG 2.0 device controllers
 *
 * Copyright (C) 2008 for Samsung Electronics
 *
 * BSP Support for Samsung's UDC driver
 * available at:
 * git://git.kernel.org/pub/scm/linux/kernel/git/kki_ap/linux-2.6-samsung.git
 *
 * State machine bugfixes:
 * Marek Szyprowski <m.szyprowski@samsung.com>
 *
 * Ported to u-boot:
 * Marek Szyprowski <m.szyprowski@samsung.com>
 * Lukasz Majewski <l.majewski@samsumg.com>
 */

#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <malloc.h>

#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>

#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include <asm/io.h>

#include <asm/mach-types.h>

#include "dwc2_udc_otg_regs.h"
#include "dwc2_udc_otg_priv.h"

#include <usb/dwc2_udc.h>

void otg_phy_init(struct dwc2_udc *dev)
{
	unsigned int usb_phy_ctrl = dev->pdata->usb_phy_ctrl;
	struct dwc2_usbotg_phy *phy =
		(struct dwc2_usbotg_phy *)dev->pdata->regs_phy;

	dev->pdata->phy_control(1);

	/* USB PHY0 Enable */
	printf("USB PHY0 Enable\n");

	/* Enable PHY */
	setbits_le32(usb_phy_ctrl, USB_PHY_CTRL_EN0);

	if (dev->pdata->usb_flags == PHY0_SLEEP) /* C210 Universal */
		clrbits_le32(&phy->phypwr, PHY_0_SLEEP | OTG_DISABLE_0 |
					   ANALOG_PWRDOWN | FORCE_SUSPEND_0);
	else /* C110 GONI */
		clrbits_le32(&phy->phypwr, OTG_DISABLE_0 | ANALOG_PWRDOWN | FORCE_SUSPEND_0);

	if (s5p_cpu_id == 0x4412)
		clrsetbits_le32(&phy->phyclk, EXYNOS4X12_ID_PULLUP0 | EXYNOS4X12_COMMON_ON_N0,
				EXYNOS4X12_CLK_SEL_24MHZ); /* PLL 24Mhz */
	else
		clrsetbits_le32(&phy->phyclk, ID_PULLUP0 | COMMON_ON_N0,
				CLK_SEL_24MHZ); /* PLL 24Mhz */

	clrsetbits_le32(&phy->rstcon, LINK_SW_RST | PHYLNK_SW_RST, PHY_SW_RST0);
	udelay(10);
	clrbits_le32(&phy->rstcon, PHY_SW_RST0 | LINK_SW_RST | PHYLNK_SW_RST);
	udelay(10);
}

void otg_phy_off(struct dwc2_udc *dev)
{
	unsigned int usb_phy_ctrl = dev->pdata->usb_phy_ctrl;
	struct dwc2_usbotg_phy *phy =
		(struct dwc2_usbotg_phy *)dev->pdata->regs_phy;

	/* reset controller just in case */
	writel(PHY_SW_RST0, &phy->rstcon);
	udelay(20);
	writel(readl(&phy->phypwr) &~PHY_SW_RST0, &phy->rstcon);
	udelay(20);

	setbits_le32(&phy->phypwr, OTG_DISABLE_0 | ANALOG_PWRDOWN | FORCE_SUSPEND_0);

	clrbits_le32(usb_phy_ctrl, USB_PHY_CTRL_EN0);

	clrbits_le32(&phy->phyclk, ID_PULLUP0 | COMMON_ON_N0);

	udelay(10000);

	dev->pdata->phy_control(0);
}
