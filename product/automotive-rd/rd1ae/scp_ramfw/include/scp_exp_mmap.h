/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Base address definitions for the SCP's expansion memory regions.
 */

#ifndef SCP_EXP_MMAP_H
#define SCP_EXP_MMAP_H

#include "scp_mmap.h"

/* PLLs are connected in SCP's expansion3 memory region */
#define SCP_PLL_BASE (SCP_SOC_EXPANSION3_BASE + 0x03000000UL)

#define SCP_PLL_SYSPLL       (SCP_PLL_BASE + 0x00000000UL)
#define SCP_PLL_INTERCONNECT (SCP_PLL_BASE + 0x00000020UL)
#define SCP_PLL_STATUS0      (SCP_PLL_BASE + 0x00000180UL)

/* PLL lock status flag mask */
#define PLL_STATUS_0_SYSPLL_LOCK (0x00000002UL)
#define PLL_STATUS_0_INTPLL_LOCK (0x00000008UL)

#endif /* SCP_EXP_MMAP_H */
