/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TC4_PPU_V1_H
#define TC4_PPU_V1_H

#include "scp_mmap.h"
#include "config_power_domain.h"
#include "tc4_core.h"

#define PPU_V1_ENUM_CORE_GEN(_core_num) \
    PPU_V1_ELEMENT_IDX_CORE##_core_num

#define PPU_V1_ENUM_CLUSTER_GEN(_cluster_num) \
    PPU_V1_ELEMENT_IDX_CLUSTER##_cluster_num

/* Elements in the PPU_V1 module. One for each:
 * - Core
 * - Cluster
 */
enum ppu_v1_dev_idx {
    TC4_FOR_EACH_CORE(PPU_V1_ENUM_CORE_GEN),
    TC4_FOR_EACH_CLUSTER(PPU_V1_ENUM_CLUSTER_GEN),
    PPU_V1_ELEMENT_IDX_MAX
};

#undef PPU_V1_ENUM_CORE_GEN
#undef PPU_V1_ENUM_CLUSTER_GEN

#endif /* TC4_PPU_V1_H */
