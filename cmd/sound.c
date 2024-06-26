// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2012 Samsung Electronics
 * Rajeshwari Shinde <rajeshwari.s@samsung.com>
 */

#include <command.h>
#include <dm.h>
#include <fdtdec.h>
#include <sound.h>
#include <asm/global_data.h>

DECLARE_GLOBAL_DATA_PTR;

/* Initilaise sound subsystem */
static int do_init(struct cmd_tbl *cmdtp, int flag, int argc,
		   char *const argv[])
{
	struct udevice *dev;
	int ret;

	ret = uclass_first_device_err(UCLASS_SOUND, &dev);
	if (!ret)
		ret = sound_setup(dev);
	if (ret) {
		printf("Initialise Audio driver failed (ret=%d)\n", ret);
		return CMD_RET_FAILURE;
	}

	return 0;
}

/* play sound from buffer */
static int do_play(struct cmd_tbl *cmdtp, int flag, int argc,
		   char *const argv[])
{
	struct udevice *dev;
	int ret = 0;
	int msec = 1000;
	int freq = 400;
	bool first = true;

	ret = uclass_first_device_err(UCLASS_SOUND, &dev);
	if (ret)
		goto err;
	--argc;
	++argv;
	while (argc || first) {
		first = false;
		if (argc) {
			msec = dectoul(argv[0], NULL);
			--argc;
			++argv;
		}
		if (argc) {
			freq = dectoul(argv[0], NULL);
			--argc;
			++argv;
		}
		ret = sound_beep(dev, msec, freq);
		if (ret)
			goto err;
	}
	return 0;

err:
	printf("Sound device failed to play (err=%d)\n", ret);
	return CMD_RET_FAILURE;
}

static struct cmd_tbl cmd_sound_sub[] = {
	U_BOOT_CMD_MKENT(init, 0, 1, do_init, "", ""),
	U_BOOT_CMD_MKENT(play, INT_MAX, 1, do_play, "", ""),
};

/* process sound command */
static int do_sound(struct cmd_tbl *cmdtp, int flag, int argc,
		    char *const argv[])
{
	struct cmd_tbl *c;

	if (argc < 1)
		return CMD_RET_USAGE;

	/* Strip off leading 'sound' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_sound_sub[0], ARRAY_SIZE(cmd_sound_sub));

	if (c)
		return c->cmd(cmdtp, flag, argc, argv);
	else
		return CMD_RET_USAGE;
}

U_BOOT_CMD(
	sound, INT_MAX, 1, do_sound,
	"sound sub-system",
	"init - initialise the sound driver\n"
	"sound play [len [freq [len [freq ...]]]] - play sounds\n"
	"  len - duration in ms\n"
	"  freq - frequency in Hz\n"
);
