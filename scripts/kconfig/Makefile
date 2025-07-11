# SPDX-License-Identifier: GPL-2.0
# ===========================================================================
# Kernel configuration targets
# These targets are used from top-level makefile

PHONY += xconfig gconfig menuconfig config localmodconfig localyesconfig \
	build_menuconfig build_nconfig build_gconfig build_xconfig

# Added for U-Boot
#  Linux has defconfig files in arch/$(SRCARCH)/configs/,
#  on the other hand, U-Boot does in configs/.
#  Set SRCARCH to .. fake this Makefile.
SRCARCH := ..

ifdef KBUILD_KCONFIG
Kconfig := $(KBUILD_KCONFIG)
else
Kconfig := Kconfig
endif

ifeq ($(quiet),silent_)
silent := -s
endif

# We need this, in case the user has it in its environment
unexport CONFIG_

xconfig: $(obj)/qconf
	$< $(silent) $(Kconfig)

gconfig: $(obj)/gconf
	$< $(silent) $(Kconfig)

menuconfig: $(obj)/mconf
	$< $(silent) $(Kconfig)

config: $(obj)/conf
	$< $(silent) --oldaskconfig $(Kconfig)

nconfig: $(obj)/nconf
	$< $(silent) $(Kconfig)

build_menuconfig: $(obj)/mconf

build_nconfig: $(obj)/nconf

build_gconfig: $(obj)/gconf

build_xconfig: $(obj)/qconf

localyesconfig localmodconfig: $(obj)/conf
	$(Q)perl $(srctree)/$(src)/streamline_config.pl --$@ $(srctree) $(Kconfig) > .tmp.config
	$(Q)if [ -f .config ]; then					\
			cmp -s .tmp.config .config ||			\
			(mv -f .config .config.old.1;			\
			 mv -f .tmp.config .config;			\
			 $< $(silent) --oldconfig $(Kconfig);		\
			 mv -f .config.old.1 .config.old)		\
	else								\
			mv -f .tmp.config .config;			\
			$< $(silent) --oldconfig $(Kconfig);		\
	fi
	$(Q)rm -f .tmp.config

# These targets map 1:1 to the commandline options of 'conf'
#
# Note:
#  syncconfig has become an internal implementation detail and is now
#  deprecated for external use
simple-targets := oldconfig allnoconfig allyesconfig allmodconfig \
	alldefconfig randconfig listnewconfig olddefconfig syncconfig
PHONY += $(simple-targets)

$(simple-targets): $(obj)/conf
	$< $(silent) --$@ $(Kconfig)

PHONY += savedefconfig defconfig

savedefconfig: $(obj)/conf
	$< $(silent) --$@=defconfig $(Kconfig)

defconfig: $(obj)/conf
ifeq ($(KBUILD_DEFCONFIG),)
	$< $(silent) --defconfig $(Kconfig)
else
ifneq ($(wildcard $(srctree)/arch/$(SRCARCH)/configs/$(KBUILD_DEFCONFIG)),)
	@$(kecho) "*** Default configuration is based on '$(KBUILD_DEFCONFIG)'"
	$(Q)$< $(silent) --defconfig=arch/$(SRCARCH)/configs/$(KBUILD_DEFCONFIG) $(Kconfig)
else
	@$(kecho) "*** Default configuration is based on target '$(KBUILD_DEFCONFIG)'"
	$(Q)$(MAKE) -f $(srctree)/Makefile $(KBUILD_DEFCONFIG)
endif
endif

%_defconfig: $(obj)/conf
	$(Q)$(CPP) -nostdinc -P -I $(srctree) -undef -x assembler-with-cpp $(srctree)/arch/$(SRCARCH)/configs/$@ -o generated_defconfig
	$(Q)sed -i -e 's/^[[:space:]]//' generated_defconfig
	$(Q)$< $(silent) --defconfig=generated_defconfig $(Kconfig)

# Added for U-Boot (backward compatibility)
%_config: %_defconfig
	@:

configfiles=$(wildcard $(srctree)/kernel/configs/$@ \
		$(srctree)/arch/$(SRCARCH)/configs/$@ \
		$(shell find $(srctree)/board -name "$@"))

%.config: $(obj)/conf
	$(if $(call configfiles),, $(error No configuration exists for this target on this architecture))
	$(Q)$(CONFIG_SHELL) $(srctree)/scripts/kconfig/merge_config.sh -m .config $(configfiles)
	+$(Q)yes "" | $(MAKE) -f $(srctree)/Makefile oldconfig

PHONY += kvmconfig
kvmconfig: kvm_guest.config
	@:

PHONY += xenconfig
xenconfig: xen.config
	@:

PHONY += tinyconfig
tinyconfig:
	$(Q)$(MAKE) -f $(srctree)/Makefile allnoconfig tiny.config

# CHECK: -o cache_dir=<path> working?
PHONY += testconfig
testconfig: $(obj)/conf
	$(PYTHON3) -B -m pytest $(srctree)/$(src)/tests \
	-o cache_dir=$(abspath $(obj)/tests/.cache) \
	$(if $(findstring 1,$(KBUILD_VERBOSE)),--capture=no)
clean-dirs += tests/.cache

# Help text used by make help
help:
	@echo  '  config	  - Update current config utilising a line-oriented program'
	@echo  '  nconfig         - Update current config utilising a ncurses menu based program'
	@echo  '  menuconfig	  - Update current config utilising a menu based program'
	@echo  '  xconfig	  - Update current config utilising a Qt based front-end'
	@echo  '  gconfig	  - Update current config utilising a GTK+ based front-end'
	@echo  '  oldconfig	  - Update current config utilising a provided .config as base'
	@echo  '  localmodconfig  - Update current config disabling modules not loaded'
	@echo  '  localyesconfig  - Update current config converting local mods to core'
	@echo  '  defconfig	  - New config with default from ARCH supplied defconfig'
	@echo  '  savedefconfig   - Save current config as ./defconfig (minimal config)'
	@echo  '  allnoconfig	  - New config where all options are answered with no'
	@echo  '  allyesconfig	  - New config where all options are accepted with yes'
	@echo  '  allmodconfig	  - New config selecting modules when possible'
	@echo  '  alldefconfig    - New config with all symbols set to default'
	@echo  '  randconfig	  - New config with random answer to all options'
	@echo  '  listnewconfig   - List new options'
	@echo  '  olddefconfig	  - Same as oldconfig but sets new symbols to their'
	@echo  '                    default value without prompting'
#	@echo  '  kvmconfig	  - Enable additional options for kvm guest kernel support'
#	@echo  '  xenconfig       - Enable additional options for xen dom0 and guest kernel support'
#	@echo  '  tinyconfig	  - Configure the tiniest possible kernel'
	@echo  '  testconfig	  - Run Kconfig unit tests (requires python3 and pytest)'

# ===========================================================================
# Shared Makefile for the various kconfig executables:
# conf:	  Used for defconfig, oldconfig and related targets
# object files used by all kconfig flavours

conf-objs	:= conf.o  zconf.tab.o

hostprogs-y := conf

targets		+= zconf.lex.c

# generated files seem to need this to find local include files
HOSTCFLAGS_zconf.lex.o	:= -I$(src)
HOSTCFLAGS_zconf.tab.o	:= -I$(src)

# nconf: Used for the nconfig target based on ncurses
hostprogs-y	+= nconf
nconf-objs	:= nconf.o zconf.tab.o nconf.gui.o

HOSTLDLIBS_nconf	= $(shell . $(obj)/.nconf-cfg && echo $$libs)
HOSTCFLAGS_nconf.o	= $(shell . $(obj)/.nconf-cfg && echo $$cflags)
HOSTCFLAGS_nconf.gui.o	= $(shell . $(obj)/.nconf-cfg && echo $$cflags)

$(obj)/nconf.o $(obj)/nconf.gui.o: $(obj)/.nconf-cfg

# mconf: Used for the menuconfig target based on lxdialog
hostprogs-y	+= mconf
lxdialog	:= checklist.o inputbox.o menubox.o textbox.o util.o yesno.o
mconf-objs	:= mconf.o zconf.tab.o $(addprefix lxdialog/, $(lxdialog))

HOSTLDLIBS_mconf = $(shell . $(obj)/.mconf-cfg && echo $$libs)
$(foreach f, mconf.o $(lxdialog), \
  $(eval HOSTCFLAGS_$f = $$(shell . $(obj)/.mconf-cfg && echo $$$$cflags)))

$(obj)/mconf.o: $(obj)/.mconf-cfg
$(addprefix $(obj)/lxdialog/, $(lxdialog)): $(obj)/.mconf-cfg

# qconf: Used for the xconfig target based on Qt
hostprogs-y	+= qconf
qconf-cxxobjs	:= qconf.o
qconf-objs	:= zconf.tab.o

HOSTLDLIBS_qconf	= $(shell . $(obj)/.qconf-cfg && echo $$libs)
HOSTCXXFLAGS_qconf.o	= $(shell . $(obj)/.qconf-cfg && echo $$cflags)

$(obj)/qconf.o: $(obj)/.qconf-cfg $(obj)/qconf.moc

quiet_cmd_moc = MOC     $@
      cmd_moc = $(shell . $(obj)/.qconf-cfg && echo $$moc) -i $< -o $@

$(obj)/%.moc: $(src)/%.h $(obj)/.qconf-cfg
	$(call cmd,moc)

# gconf: Used for the gconfig target based on GTK+
hostprogs-y	+= gconf
gconf-objs	:= gconf.o zconf.tab.o

HOSTLDLIBS_gconf = $(shell . $(obj)/.gconf-cfg && echo $$libs)
HOSTCFLAGS_gconf.o  = $(shell . $(obj)/.gconf-cfg && echo $$cflags)

$(obj)/gconf.o: $(obj)/.gconf-cfg

$(obj)/zconf.tab.o: $(obj)/zconf.lex.c

# check if necessary packages are available, and configure build flags
filechk_conf_cfg = $(CONFIG_SHELL) $<

$(obj)/.%conf-cfg: $(src)/%conf-cfg.sh FORCE
	$(call filechk,conf_cfg)

clean-files += .*conf-cfg
