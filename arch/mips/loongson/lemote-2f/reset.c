/* Board-specific reboot/shutdown routines
 *
 * Copyright (c) 2009 Philippe Vachon <philippe@cowpig.ca>
 *
 * Copyright (C) 2009 Lemote Inc.
 * Author: Wu Zhangjin, wuzhangjin@gmail.com
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/io.h>
#include <linux/delay.h>
#include <linux/types.h>

#include <asm/bootinfo.h>

#include <loongson.h>

#include <cs5536/cs5536.h>
#include "ec_kb3310b.h"

static void reset_cpu(void)
{
	LOONGSON_CHIPCFG0 |= 0x7;
}


static void fl2f_reboot(void)
{
	reset_cpu();

	{
		u32 hi, lo;
		_rdmsr(DIVIL_MSR_REG(DIVIL_SOFT_RESET), &hi, &lo);
		lo |= 0x00000001;
		_wrmsr(DIVIL_MSR_REG(DIVIL_SOFT_RESET), hi, lo);
	}
}

static void fl2f_shutdown(void)
{
	u32 hi, lo, val;
	int gpio_base;

	
	_rdmsr(DIVIL_MSR_REG(DIVIL_LBAR_GPIO), &hi, &lo);
	gpio_base = lo & 0xff00;

	
	val = inl(gpio_base + GPIOL_OUT_EN);
	val &= ~(1 << (16 + 13));
	val |= (1 << 13);
	outl(val, gpio_base + GPIOL_OUT_EN);
	mmiowb();
	
	val = inl(gpio_base + GPIOL_OUT_VAL) & ~(1 << (13));
	val |= (1 << (16 + 13));
	outl(val, gpio_base + GPIOL_OUT_VAL);
	mmiowb();
}


void ml2f_reboot(void)
{
	reset_cpu();

	
	ec_write(REG_RESET, BIT_RESET_ON);
}

#define yl2f89_reboot ml2f_reboot

#define EC_SHUTDOWN_IO_PORT_HIGH 0xff2d
#define EC_SHUTDOWN_IO_PORT_LOW	 0xff2e
#define EC_SHUTDOWN_IO_PORT_DATA 0xff2f
#define REG_SHUTDOWN_HIGH        0xFC
#define REG_SHUTDOWN_LOW         0x29
#define BIT_SHUTDOWN_ON          (1 << 1)

static void ml2f_shutdown(void)
{
	u8 val;
	u64 i;

	outb(REG_SHUTDOWN_HIGH, EC_SHUTDOWN_IO_PORT_HIGH);
	outb(REG_SHUTDOWN_LOW, EC_SHUTDOWN_IO_PORT_LOW);
	mmiowb();
	val = inb(EC_SHUTDOWN_IO_PORT_DATA);
	outb(val & (~BIT_SHUTDOWN_ON), EC_SHUTDOWN_IO_PORT_DATA);
	mmiowb();
	
	for (i = 0; i < 0x10000; i++)
		delay();
	outb(val | BIT_SHUTDOWN_ON, EC_SHUTDOWN_IO_PORT_DATA);
	mmiowb();
}

static void yl2f89_shutdown(void)
{
	
	LOONGSON_GPIODATA &= ~0x00000001;
	
	LOONGSON_GPIOIE &= ~0x00000001;
}

void mach_prepare_reboot(void)
{
	switch (mips_machtype) {
	case MACH_LEMOTE_FL2F:
	case MACH_LEMOTE_NAS:
	case MACH_LEMOTE_LL2F:
		fl2f_reboot();
		break;
	case MACH_LEMOTE_ML2F7:
		ml2f_reboot();
		break;
	case MACH_LEMOTE_YL2F89:
		yl2f89_reboot();
		break;
	default:
		break;
	}
}

void mach_prepare_shutdown(void)
{
	switch (mips_machtype) {
	case MACH_LEMOTE_FL2F:
	case MACH_LEMOTE_NAS:
	case MACH_LEMOTE_LL2F:
		fl2f_shutdown();
		break;
	case MACH_LEMOTE_ML2F7:
		ml2f_shutdown();
		break;
	case MACH_LEMOTE_YL2F89:
		yl2f89_shutdown();
		break;
	default:
		break;
	}
}
