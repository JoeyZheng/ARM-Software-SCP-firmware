/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ccsm_drv.h"

#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_status.h>

#include <stddef.h>
#include <stdint.h>

#define MOD_CCSM_DRV_REG(base, offset) *((volatile uint32_t *)(base + offset))

/*
 * Static helper functions
 */

static int ccsm_clear_irq_status(uint32_t base_address)
{
    uint32_t clear_mask;
    uint32_t wait_cycles = MOD_CCSM_CLEAR_IRQ_TIMEOUT;

    clear_mask = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_IRQ_STATUS_OFFSET);
    if (clear_mask != 0) {
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_IRQ_STAT_CLR_OFFSET) =
            clear_mask;
    }

    while (MOD_CCSM_DRV_REG(base_address, MOD_CCSM_IRQ_STATUS_OFFSET) != 0) {
        wait_cycles--;
        if (wait_cycles == 0) {
            return FWK_E_TIMEOUT;
        }
    }

    return FWK_SUCCESS;
}

static int ccsm_clear_irq_error(uint32_t base_address)
{
    uint32_t clear_mask;
    uint32_t wait_cycles = MOD_CCSM_CLEAR_IRQ_TIMEOUT;

    clear_mask = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_IRQ_ERROR_OFFSET);
    if (clear_mask != 0) {
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_IRQ_ERR_CLR_OFFSET) =
            clear_mask;
    }

    while (MOD_CCSM_DRV_REG(base_address, MOD_CCSM_IRQ_ERROR_OFFSET) != 0) {
        wait_cycles--;
        if (wait_cycles == 0) {
            return FWK_E_TIMEOUT;
        }
    }

    return FWK_SUCCESS;
}

/*! Send a request to the CCSM and await completion. */
static int ccsm_send_await_request(
    uint32_t base_address,
    enum mod_ccsm_req_type req)
{
    int ret = FWK_SUCCESS;
    uint32_t wait_cycles = MOD_CCSM_COMMAND_WAIT_TIMEOUT;

    if (MOD_CCSM_DRV_REG(base_address, MOD_CCSM_REQ_TYPE_STATUS_OFFSET) != 0) {
        return FWK_E_DEVICE;
    }
    if ((MOD_CCSM_DRV_REG(base_address, MOD_CCSM_REQ_STATUS_OFFSET) &
         MOD_CCSM_REQ_OG) != 0) {
        return FWK_E_BUSY;
    }

    MOD_CCSM_DRV_REG(base_address, MOD_CCSM_REQ_TYPE_OFFSET) = req;
    MOD_CCSM_DRV_REG(base_address, MOD_CCSM_REQ_EN_OFFSET) =
        MOD_CCSM_REQ_EN_SET;

    while ((MOD_CCSM_DRV_REG(base_address, MOD_CCSM_REQ_STATUS_OFFSET) &
            MOD_CCSM_REQ_OG) != 0) {
        wait_cycles--;
        if (wait_cycles == 0) {
            return FWK_E_TIMEOUT;
        }
    }

    /* Clear IRQ status bits set on completion. */
    ret = ccsm_clear_irq_status(base_address);
    if (ret != FWK_SUCCESS) {
        return ret;
    }

    MOD_CCSM_DRV_REG(base_address, MOD_CCSM_REQ_TYPE_OFFSET) =
        MOD_CCSM_REQ_TYPE_CLR;
    MOD_CCSM_DRV_REG(base_address, MOD_CCSM_REQ_EN_OFFSET) =
        MOD_CCSM_REQ_EN_CLR;
    return FWK_SUCCESS;
}

/*
 * CCSM driver functions
 */

int ccsm_drv_get_pll_settings(
    uint32_t base_address,
    struct mod_ccsm_pll_reg_values *config)
{
    config->pll_nom_settings_0 =
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_NOM_0_STATUS_OFFSET);
    config->pll_nom_settings_1 =
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_NOM_1_STATUS_OFFSET);
    config->pll_fb_settings_0 =
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_FB_0_STATUS_OFFSET);
    config->pll_fb_settings_1 =
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_FB_1_STATUS_OFFSET);
    config->pll_0_static =
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_STATIC_0_STATUS_OFFSET);
    config->pll_1_static =
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_STATIC_1_STATUS_OFFSET);

    return FWK_SUCCESS;
}

int ccsm_drv_set_pll_static_settings(
    uint32_t base_address,
    enum mod_ccsm_pll_static_select pll_select,
    uint32_t config)
{
    if (pll_select == MOD_CCSM_PLL_0) {
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_STATIC_0_OFFSET) = config;
    } else if (pll_select == MOD_CCSM_PLL_1) {
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_STATIC_1_OFFSET) = config;
    } else {
        return FWK_E_PARAM;
    }
    return FWK_SUCCESS;
}

int ccsm_drv_set_pll_dynamic_settings(
    uint32_t base_address,
    enum mod_ccsm_pll_dynamic_select pll_select,
    uint32_t config0,
    uint32_t config1)
{
    if (pll_select == MOD_CCSM_PLL_NOMINAL) {
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_NOM_0_OFFSET) = config0;
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_NOM_1_OFFSET) = config1;
    } else if (pll_select == MOD_CCSM_PLL_FALLBACK) {
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_FB_0_OFFSET) = config0;
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_PLL_FB_1_OFFSET) = config1;
    } else {
        return FWK_E_PARAM;
    }
    return FWK_SUCCESS;
}

int ccsm_drv_get_dm_configuration(uint32_t base_address, uint32_t *config)
{
    *config =
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_DM_STRATEGY_STATUS_OFFSET);
    return FWK_SUCCESS;
}

int ccsm_drv_set_dm_configuration(uint32_t base_address, uint32_t config)
{
    MOD_CCSM_DRV_REG(base_address, MOD_CCSM_DM_STRATEGY_OFFSET) = config;
    return FWK_SUCCESS;
}

int ccsm_drv_get_dm_telemetry(
    uint32_t base_address,
    uint32_t *telemetry1,
    uint32_t *telemetry2,
    uint32_t *telemetry3)
{
    *telemetry1 = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_DM_TELEMETRY1_OFFSET);
    *telemetry2 = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_DM_TELEMETRY2_OFFSET);
    *telemetry3 = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_DM_TELEMETRY3_OFFSET);

    return FWK_SUCCESS;
}

int ccsm_drv_set_mod_configuration(
    uint32_t base_address,
    uint32_t core_id,
    uint32_t config)
{
    switch (core_id) {
    case 0:
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_MOD_SETTINGS_0_OFFSET) = config;
        break;
    case 1:
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_MOD_SETTINGS_1_OFFSET) = config;
        break;
    case 2:
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_MOD_SETTINGS_2_OFFSET) = config;
        break;
    case 3:
        MOD_CCSM_DRV_REG(base_address, MOD_CCSM_MOD_SETTINGS_3_OFFSET) = config;
        break;
    default:
        return FWK_E_PARAM;
    }
    return FWK_SUCCESS;
}

int ccsm_drv_get_mod_configuration(
    uint32_t base_address,
    uint32_t core_id,
    uint32_t *config)
{
    switch (core_id) {
    case 0:
        *config = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_MOD_STATUS_0_OFFSET);
        break;
    case 1:
        *config = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_MOD_STATUS_1_OFFSET);
        break;
    case 2:
        *config = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_MOD_STATUS_2_OFFSET);
        break;
    case 3:
        *config = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_MOD_STATUS_3_OFFSET);
        break;
    default:
        return FWK_E_PARAM;
    }
    return FWK_SUCCESS;
}

int ccsm_drv_get_irq_error(uint32_t base_address, uint32_t *error)
{
    *error = MOD_CCSM_DRV_REG(base_address, MOD_CCSM_IRQ_ERROR_OFFSET);

    return ccsm_clear_irq_error(base_address);
}

int ccsm_drv_req_set_dvfs(uint32_t base_address)
{
    return ccsm_send_await_request(base_address, MOD_CCSM_REQ_SET_DVFS);
}

int ccsm_drv_req_set_dm(uint32_t base_address)
{
    return ccsm_send_await_request(base_address, MOD_CCSM_REQ_SET_DM);
}

int ccsm_drv_req_set_mod(uint32_t base_address)
{
    return ccsm_send_await_request(base_address, MOD_CCSM_REQ_SET_MOD);
}

int ccsm_drv_req_clr_mod(uint32_t base_address)
{
    return ccsm_send_await_request(base_address, MOD_CCSM_REQ_CLR_MOD);
}

int ccsm_drv_req_debug(uint32_t base_address)
{
    return ccsm_send_await_request(base_address, MOD_CCSM_REQ_DBG);
}

int ccsm_drv_req_dvfs_go_up(uint32_t base_address)
{
    int ret;

    ret = ccsm_send_await_request(base_address, MOD_CCSM_REQ_GO_UP);
    if (ret != FWK_SUCCESS) {
        return ret;
    }
    return ccsm_send_await_request(base_address, MOD_CCSM_REQ_GO_UP2);
}

int ccsm_drv_req_dvfs_go_dn(uint32_t base_address)
{
    int ret;

    ret = ccsm_send_await_request(base_address, MOD_CCSM_REQ_GO_DN);
    if (ret != FWK_SUCCESS) {
        return ret;
    }
    return ccsm_send_await_request(base_address, MOD_CCSM_REQ_GO_DN2);
}
