/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CCSM_DRV_H
#define CCSM_DRV_H

#include <mod_ccsm.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_macros.h>

#include <stdbool.h>
#include <stdint.h>

/*! CCSM timeout durations. */
#define MOD_CCSM_CLEAR_IRQ_TIMEOUT    1000000
#define MOD_CCSM_COMMAND_WAIT_TIMEOUT 1000000

/*! PLL programmable register offsets. */
#define MOD_CCSM_PLL_NOM_0_OFFSET    0x004
#define MOD_CCSM_PLL_NOM_1_OFFSET    0x008
#define MOD_CCSM_PLL_FB_0_OFFSET     0x014
#define MOD_CCSM_PLL_FB_1_OFFSET     0x018
#define MOD_CCSM_PLL_STATIC_0_OFFSET 0x020
#define MOD_CCSM_PLL_STATIC_1_OFFSET 0x024

/*! PLL status register offsets. */
#define MOD_CCSM_PLL_NOM_0_STATUS_OFFSET    0x104
#define MOD_CCSM_PLL_NOM_1_STATUS_OFFSET    0x108
#define MOD_CCSM_PLL_FB_0_STATUS_OFFSET     0x114
#define MOD_CCSM_PLL_FB_1_STATUS_OFFSET     0x118
#define MOD_CCSM_PLL_STATIC_0_STATUS_OFFSET 0x120
#define MOD_CCSM_PLL_STATIC_1_STATUS_OFFSET 0x124

/*! Droop mitigation register offsets. */
#define MOD_CCSM_DM_STRATEGY_OFFSET        0x060
#define MOD_CCSM_DM_STRATEGY_STATUS_OFFSET 0x160
#define MOD_CCSM_DM_TELEMETRY1_OFFSET      0x1C0
#define MOD_CCSM_DM_TELEMETRY2_OFFSET      0x1C4
#define MOD_CCSM_DM_TELEMETRY3_OFFSET      0x1C8

/*! Modulator programmable register offsets. */
#define MOD_CCSM_MOD_SETTINGS_0_OFFSET 0x070
#define MOD_CCSM_MOD_SETTINGS_1_OFFSET 0x074
#define MOD_CCSM_MOD_SETTINGS_2_OFFSET 0x078
#define MOD_CCSM_MOD_SETTINGS_3_OFFSET 0x07C

/*! Modulator status register offsets. */
#define MOD_CCSM_MOD_STATUS_0_OFFSET 0x170
#define MOD_CCSM_MOD_STATUS_1_OFFSET 0x174
#define MOD_CCSM_MOD_STATUS_2_OFFSET 0x178
#define MOD_CCSM_MOD_STATUS_3_OFFSET 0x17C

/*! MOD_CCSM request register offsets. */
#define MOD_CCSM_REQ_TYPE_OFFSET        0x090
#define MOD_CCSM_REQ_EN_OFFSET          0x0A0
#define MOD_CCSM_REQ_TYPE_STATUS_OFFSET 0x190
#define MOD_CCSM_REQ_STATUS_OFFSET      0x1A0

/*! State machine status register offsets. */
#define MOD_CCSM_CONTROL_SM_STATE_OFFSET 0x1B0
#define MOD_CCSM_DVFS_SM_STATE_OFFSET    0x1B4
#define MOD_CCSM_DM_SM_STATE_OFFSET      0x1B8
#define MOD_CCSM_OC_SM_STATE_OFFSET      0x1BC

/*! IRQ register offsets. */
#define MOD_CCSM_IRQ_STAT_MASK_OFFSET 0x0A4
#define MOD_CCSM_IRQ_STAT_CLR_OFFSET  0x0B4
#define MOD_CCSM_IRQ_ERR_CLR_OFFSET   0x0B8
#define MOD_CCSM_IRQ_STATUS_OFFSET    0x1A4
#define MOD_CCSM_IRQ_ERROR_OFFSET     0x1A8

/*!
 * \brief Static configuration PLL selection.
 */
enum mod_ccsm_pll_static_select { MOD_CCSM_PLL_0, MOD_CCSM_PLL_1 };

/*!
 * \brief Dynamic configuration PLL selection.
 */
enum mod_ccsm_pll_dynamic_select {
    MOD_CCSM_PLL_NOMINAL,
    MOD_CCSM_PLL_FALLBACK
};

/*!
 * \brief Values for request type register.
 */
enum mod_ccsm_req_type {
    MOD_CCSM_REQ_TYPE_CLR = 0,
    MOD_CCSM_REQ_SET_DVFS = 1 << 0,
    MOD_CCSM_REQ_GO_UP = 1 << 1,
    MOD_CCSM_REQ_GO_DN = 1 << 2,
    MOD_CCSM_REQ_GO_UP2 = 1 << 3,
    MOD_CCSM_REQ_GO_DN2 = 1 << 4,
    MOD_CCSM_REQ_SET_DM = 1 << 8,
    MOD_CCSM_REQ_CLR_MOD = 1 << 16,
    MOD_CCSM_REQ_SET_MOD = 1 << 17,
    MOD_CCSM_REQ_DBG = 1 << 24
};

/*!
 * \brief Values for request enable register.
 */
enum mod_ccsm_req_enable { MOD_CCSM_REQ_EN_CLR = 0, MOD_CCSM_REQ_EN_SET = 1 };

/*!
 * \brief Values for request status register.
 */
enum mod_ccsm_req_status { MOD_CCSM_REQ_OG = 1 };

/*!
 * \brief Value offsets in droop mitigation register.
 */
enum mod_ccsm_dm_offset {
    MOD_CCSM_DM_TRANSITION_PAUSE = 0,
    MOD_CCSM_DM_MITIGATION_DURATION = 4
};

/*!
 * \brief Value masks for droop mitigation register.
 */
enum mod_ccsm_dm_mask {
    MOD_CCSM_DM_TRANSITION_PAUSE_MASK = 0x0F,
    MOD_CCSM_DM_MITIGATION_DURATION_MASK = 0xFFF0
};

/*!
 * \brief Value offsets in modulator registers.
 */
enum mod_ccsm_mod_offset {
    MOD_CCSM_MOD_NUMERATOR = 0,
    MOD_CCSM_MOD_DENOMINATOR = 8,
    MOD_CCSM_MOD_NUMERATOR_PMIC_OC = 16
};

/*!
 * \brief Value masks for modulator registers.
 */
enum mod_ccsm_mod_mask {
    MOD_CCSM_MOD_NUMERATOR_MASK = 0x0000FF,
    MOD_CCSM_MOD_DENOMINATOR_MASK = 0x00FF00,
    MOD_CCSM_MOD_NUMERATOR_PMIC_OC_MASK = 0xFF0000
};

/*!
 * \brief Status values for control state machine.
 */
enum mod_ccsm_control_sm_status {
    MOD_CCSM_CONTROL_SM_CURRENT_STATE = 1 << 0,
    MOD_CCSM_CONTROL_SM_PREV_STATE = 1 << 8,
    MOD_CCSM_CONTROL_SM_BEFORE_PREV_STATE = 1 << 16,
    MOD_CCSM_CONTROL_SM_CLKSEL_NOM0 = 1 << 24,
    MOD_CCSM_CONTROL_SM_CLKSEL_NOM1 = 1 << 25,
    MOD_CCSM_CONTROL_SM_CLKSEL_FB0 = 1 << 26,
    MOD_CCSM_CONTROL_SM_CLKSEL_FB1 = 1 << 27,
    MOD_CCSM_CONTROL_SM_PLL_SWAP = 1 << 28
};

/*!
 * \brief Status values for DVFS state machine.
 */
enum mod_ccsm_dvfs_sm_status {
    MOD_CCSM_DVFS_SM_CURRENT_STATE = 1 << 0,
    MOD_CCSM_DVFS_SM_PREV_STATE = 1 << 8,
    MOD_CCSM_DVFS_SM_BEFORE_PREV_STATE = 1 << 16,
    MOD_CCSM_DVFS_GO_UP2_REQD = 1 << 24,
    MOD_CCSM_DVFS_GO_DN2_REQD = 1 << 25
};

/*!
 * \brief Status values for droop mitigation state machine.
 */
enum mod_ccsm_dm_sm_status {
    MOD_CCSM_DM_SM_CURRENT_STATE = 1 << 0,
    MOD_CCSM_DM_SM_CLKSEL = 1 << 8,
};

/*!
 * \brief Status values for overcurrent state machine.
 */
enum mod_ccsm_oc_sm_status { MOD_CCSM_OC_SM_CURRENT_STATE = 1 << 0 };

/*!
 * \brief CCSM driver functions
 */

/*!
 * \brief Get PLL configuration registers.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param[out] config Pointer to struct with current PLL register values.
 *
 */
int ccsm_drv_get_pll_settings(
    uint32_t base_address,
    struct mod_ccsm_pll_reg_values *config);

/*!
 * \brief Set static config for a PLL.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param pll_select Selector for which physical PLL to update.
 *
 * \param config The register value to be set for static config of PLL.
 *
 */
int ccsm_drv_set_pll_static_settings(
    uint32_t base_address,
    enum mod_ccsm_pll_static_select pll_select,
    uint32_t config);

/*!
 * \brief Set dynamic config for a PLL.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param pll_select Selector for which PLL to update, nominal or fallback.
 *
 * \param config0 The register value to be set for settings 0.
 *
 * \param config1 The register value to be set for settings 1.
 *
 */
int ccsm_drv_set_pll_dynamic_settings(
    uint32_t base_address,
    enum mod_ccsm_pll_dynamic_select pll_select,
    uint32_t config0,
    uint32_t config1);

/*!
 * \brief Get current droop mitigation config.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param[out] config Contents of current droop mitigation config register.
 *
 */
int ccsm_drv_get_dm_configuration(uint32_t base_address, uint32_t *config);

/*!
 * \brief Get droop mitigation telemetry. Requires debug to be enabled.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param[out] telemetry1 Contents of telemetry register 1.
 *
 * \param[out] telemetry2 Contents of telemetry register 2.
 *
 * \param[out] telemetry3 Contents of telemetry register 3.
 *
 */
int ccsm_drv_get_dm_telemetry(
    uint32_t base_address,
    uint32_t *telemetry1,
    uint32_t *telemetry2,
    uint32_t *telemetry3);

/*!
 * \brief Set target droop mitigation config, to be applied in next DVFS/DM
 * command.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param[in] config The target droop mitigation config register value to set.
 *
 */
int ccsm_drv_set_dm_configuration(uint32_t base_address, uint32_t config);

/*!
 * \brief Get current modulator config for a core.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param core_id Identifier for the core.
 *
 * \param[out] config The current modulator config register value.
 *
 */
int ccsm_drv_get_mod_configuration(
    uint32_t base_address,
    uint32_t core_id,
    uint32_t *config);

/*!
 * \brief Set modulator configuration, to be applied in next set_mod command.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param core_id Identifier for the core to be updated.
 *
 * \param config The target modulator config register value to set.
 *
 */
int ccsm_drv_set_mod_configuration(
    uint32_t base_address,
    uint32_t core_id,
    uint32_t config);

/*!
 * \brief Get IRQ error register and clear.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param[out] error The IRQ error register contents.
 *
 */
int ccsm_drv_get_irq_error(uint32_t base_address, uint32_t *error);

/*!
 * \brief Update the DVFS state machine config.
 *
 * \param base_address Base address of the CCSM.
 *
 */
int ccsm_drv_req_set_dvfs(uint32_t base_address);

/*!
 * \brief Update the droop mitigation state machine config.
 *
 * \param base_address Base address of the CCSM.
 *
 */
int ccsm_drv_req_set_dm(uint32_t base_address);

/*!
 * \brief Update the modulators/OC state machine config.
 *
 * \param base_address Base address of the CCSM.
 *
 */
int ccsm_drv_req_set_mod(uint32_t base_address);

/*!
 * \brief Clear modulator and reset the OC state machine.
 *
 * \param base_address Base address of the CCSM.
 *
 */
int ccsm_drv_req_clr_mod(uint32_t base_address);

/*!
 * \brief Enable debug registers and set the DM telemetry registers.
 *
 * \param base_address Base address of the CCSM.
 *
 */
int ccsm_drv_req_debug(uint32_t base_address);

/*!
 * \brief Trigger a CCSM DVFS_GO_UP request.
 *
 * \param base_address Base address of the CCSM.
 *
 */
int ccsm_drv_req_dvfs_go_up(uint32_t base_address);

/*!
 * \brief Trigger a CCSM DVFS_GO_DN request.
 *
 * \param base_address Base address of the CCSM.
 *
 */
int ccsm_drv_req_dvfs_go_dn(uint32_t base_address);

#endif /* CCSM_DRV_H */
