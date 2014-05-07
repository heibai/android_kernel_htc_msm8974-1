/*
 *  Copyright 2004-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __MACH_MXS_IRQS_H__
#define __MACH_MXS_IRQS_H__

#define MXS_INTERNAL_IRQS	128

#define MXS_GPIO_IRQ_START	MXS_INTERNAL_IRQS

#define MXS_GPIO_IRQS		(32 * 5)

#define MXS_BOARD_IRQ_START	(MXS_GPIO_IRQ_START + MXS_GPIO_IRQS)
#define MXS_BOARD_IRQS		16

#define NR_IRQS			(MXS_BOARD_IRQ_START + MXS_BOARD_IRQS)

#endif 
