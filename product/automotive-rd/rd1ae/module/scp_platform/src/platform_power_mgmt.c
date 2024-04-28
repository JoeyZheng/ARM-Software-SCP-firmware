/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     SCP Platform Support - Power Management
 */

#include <internal/scp_platform.h>

#include <mod_system_power.h>

#include <fwk_status.h>

#include <fmw_cmsis.h>

/* System shutdown function */
static int platform_shutdown(enum mod_pd_system_shutdown system_shutdown)
{
    while (1) {
        __WFI();
    }

    return FWK_E_DEVICE;
}

/* Module 'system_power' driver interface */
const struct mod_system_power_driver_api platform_system_pwr_drv_api = {
    .system_shutdown = platform_shutdown,
};

const void *get_platform_system_power_driver_api(void)
{
    return &platform_system_pwr_drv_api;
}

int platform_power_mgmt_bind(void)
{
    /* To be implemented */
    return FWK_E_HANDLER;
}

int init_ap_core(uint8_t core_idx)
{
    /* To be implemented */
    return FWK_E_ACCESS;
}
