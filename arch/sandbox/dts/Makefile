# SPDX-License-Identifier: GPL-2.0+

ifdef CONFIG_SANDBOX64
dtb-$(CONFIG_SANDBOX) += sandbox64.dtb
else
dtb-$(CONFIG_SANDBOX) += sandbox.dtb
endif
dtb-$(CONFIG_UT_DM) += test.dtb other.dtb
dtb-$(CONFIG_CMD_EXTENSION) += overlay0.dtbo overlay1.dtbo

include $(srctree)/scripts/Makefile.dts

# Add any required device tree compiler flags here
DTC_FLAGS += -R 4
