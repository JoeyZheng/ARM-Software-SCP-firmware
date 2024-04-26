/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Base address definitions for the SCP's sub-system.
 */

#ifndef SCP_MMAP_H
#define SCP_MMAP_H

/* Base address and size of SCP's ITCM */
#define SCP_ITC_RAM_BASE (0x00000000)
#define SCP_ITC_RAM_SIZE (256 * 1024)

/* Base address and size of SCP's DTCM */
#define SCP_DTC_RAM_BASE (0x20000000)
#define SCP_DTC_RAM_SIZE (256 * 1024)

/* SCP sub-system peripherals */
#define SCP_POWER_CONTROL_BASE        (0x50000000UL)
#define SCP_PPU_SYS0_BASE             (0x50041000UL)
#define SCP_REFCLK_CNTCONTROL_BASE    (0x2A430000UL)
#define SCP_REFCLK_CNTCTL_BASE        (0x44000000UL)
#define SCP_REFCLK_CNTBASE0_BASE      (0x44001000UL)
#define SCP_SYSTEM_PIK_BASE           (0x50040000UL)
#define SCP_UART_BASE                 (0x44002000UL)

/* Base address of SCP expansion memory regions */
#define SCP_SOC_EXPANSION3_BASE (0x40000000UL) /* 64MB size */

/* SCP addresses mapped via ATU into address translation windows */
#define SCP_ADDRESS_TRANSLATION_WINDOW0_BASE (0x60000000UL)

/*
 * Offsets within SCP's Address Translation Window0
 *          __________________________
 *         |                          |
 *         |     CLUSTER UTIL 256M    |
 *         |__________________________| 0x60000000
 */

#define SCP_ATW0_CLUSTER_UTILITY_BASE SCP_ADDRESS_TRANSLATION_WINDOW0_BASE

/*
 * Size of SCP's view of per-cluster utility memory region.
 */
#define SCP_CLUSTER_UTILITY_SIZE (0x200000UL)

/*
 * Offsets of various blocks within cluster utility that is
 * mapped into SCP's address translation window 0. These offsets are applicable
 * to each cluster in the system.
 */
#define SCP_CLUSTER_UTILITY_CORE_MANAGER_OFFSET (0x80000UL)
#define SCP_CLUSTER_UTILITY_CLUSTER_PPU_OFFSET  (0x130000UL)
#define SCP_CLUSTER_UTILITY_CORE_PPU_OFFSET     (0x180000UL)

/* Core Manager base address for a cluster 'n' */
#define SCP_CLUSTER_UTILITY_CORE_MANAGER_BASE(n) \
    (SCP_ATW0_CLUSTER_UTILITY_BASE + (n * SCP_CLUSTER_UTILITY_SIZE) + \
     SCP_CLUSTER_UTILITY_CORE_MANAGER_OFFSET)

/* Cluster PPU base address */
#define SCP_CLUSTER_UTILITY_CLUSTER_PPU_BASE(n) \
    (SCP_ATW0_CLUSTER_UTILITY_BASE + (n * SCP_CLUSTER_UTILITY_SIZE) + \
     SCP_CLUSTER_UTILITY_CLUSTER_PPU_OFFSET)

/* Application core PPU base address */
#define SCP_CLUSTER_UTILITY_CORE_PPU_BASE(n) \
    (SCP_ATW0_CLUSTER_UTILITY_BASE + (n * SCP_CLUSTER_UTILITY_SIZE) + \
     SCP_CLUSTER_UTILITY_CORE_PPU_OFFSET)

#endif /* SCP_MMAP_H */
