/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Configuration data for module 'armv7m_mpu'.
 */

#include "scp_mmap.h"

#include <mod_armv7m_mpu.h>

#include <fwk_macros.h>
#include <fwk_module.h>

#define SCP_MPU_REGION_COUNT 3

static const ARM_MPU_Region_t regions[SCP_MPU_REGION_COUNT] = {
    {
        /* 0x0000_0000 - 0xFFFF_FFFF */
        .RBAR = ARM_MPU_RBAR(0x0UL, 0x00000000UL),
        .RASR = ARM_MPU_RASR(
            1,
            ARM_MPU_AP_PRIV,
            0,
            1,
            0,
            1,
            0,
            ARM_MPU_REGION_SIZE_4GB),
    },
    {
        /* 0x0000_0000 - 0x0003_FFFF */
        .RBAR = ARM_MPU_RBAR(0x1UL, SCP_ITC_RAM_BASE),
        .RASR = ARM_MPU_RASR(
            0,
            ARM_MPU_AP_PRO,
            0,
            0,
            1,
            0,
            0,
            ARM_MPU_REGION_SIZE_256KB),
    },
    {
        /* 0x2000_0000 - 0x2003_FFFF */
        .RBAR = ARM_MPU_RBAR(0x2UL, SCP_DTC_RAM_BASE),
        .RASR = ARM_MPU_RASR(
            1,
            ARM_MPU_AP_PRIV,
            0,
            0,
            1,
            1,
            0,
            ARM_MPU_REGION_SIZE_256KB),
    },
};

const struct fwk_module_config config_armv7m_mpu = {
    .data = &((struct mod_armv7m_mpu_config){
        .region_count = FWK_ARRAY_SIZE(regions),
        .regions = regions,
    }),
};
