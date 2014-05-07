/*
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file "COPYING" in the main directory of this
 * archive for more details.
 *
 * Copyright (C) 2000 - 2001 by Kanoj Sarcar (kanoj@sgi.com)
 * Copyright (C) 2000 - 2001 by Silicon Graphics, Inc.
 * Copyright (C) 2000, 2001, 2002 Ralf Baechle
 * Copyright (C) 2000, 2001 Broadcom Corporation
 */
#ifndef __ASM_SMP_H
#define __ASM_SMP_H

#include <linux/bitops.h>
#include <linux/linkage.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/cpumask.h>

#include <linux/atomic.h>
#include <asm/smp-ops.h>

extern int smp_num_siblings;
extern cpumask_t cpu_sibling_map[];

#define raw_smp_processor_id() (current_thread_info()->cpu)

extern int __cpu_number_map[NR_CPUS];
#define cpu_number_map(cpu)  __cpu_number_map[cpu]

extern int __cpu_logical_map[NR_CPUS];
#define cpu_logical_map(cpu)  __cpu_logical_map[cpu]

#define NO_PROC_ID	(-1)

#define SMP_RESCHEDULE_YOURSELF	0x1	
#define SMP_CALL_FUNCTION	0x2
#define SMP_ICACHE_FLUSH	0x4

extern volatile cpumask_t cpu_callin_map;

extern void asmlinkage smp_bootstrap(void);

static inline void smp_send_reschedule(int cpu)
{
	extern struct plat_smp_ops *mp_ops;	

	mp_ops->send_ipi_single(cpu, SMP_RESCHEDULE_YOURSELF);
}

#ifdef CONFIG_HOTPLUG_CPU
static inline int __cpu_disable(void)
{
	extern struct plat_smp_ops *mp_ops;     

	return mp_ops->cpu_disable();
}

static inline void __cpu_die(unsigned int cpu)
{
	extern struct plat_smp_ops *mp_ops;     

	mp_ops->cpu_die(cpu);
}

extern void play_dead(void);
#endif

extern asmlinkage void smp_call_function_interrupt(void);

static inline void arch_send_call_function_single_ipi(int cpu)
{
	extern struct plat_smp_ops *mp_ops;     

	mp_ops->send_ipi_mask(&cpumask_of_cpu(cpu), SMP_CALL_FUNCTION);
}

static inline void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
	extern struct plat_smp_ops *mp_ops;     

	mp_ops->send_ipi_mask(mask, SMP_CALL_FUNCTION);
}

#endif 
