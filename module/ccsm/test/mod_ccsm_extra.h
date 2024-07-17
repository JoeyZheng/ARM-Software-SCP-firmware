/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ccsm_drv.h>

#include <mod_ccsm.h>

/*!
 * \brief Generate values for the PLL dynamic configuration registers.
 *
 * \param dev_id CCSM PLL device identifier.
 *
 * \param[in] config Target DVFS configuration.
 *
 * \param[inout] pll_reg_values Updated register values.
 *
 */
int mod_ccsm_pll_gen_dynamic_registers(
    fwk_id_t dev_id,
    const struct mod_ccsm_clock_rate *config,
    struct mod_ccsm_pll_reg_values *pll_reg_values);

/*!
 * \brief Generate values for the PLL static configuration registers.
 *
 * \param dev_id CCSM PLL device identifier.
 *
 * \param[inout] pll_reg_values Updated register values.
 *
 */
int mod_ccsm_pll_gen_static_registers(
    fwk_id_t dev_id,
    struct mod_ccsm_pll_reg_values *pll_reg_values);

/* Driver functions*/

int ccsm_send_await_request(uint32_t base_address, enum mod_ccsm_req_type req);

int ccsm_clear_irq_status(uint32_t base_address);

int ccsm_clear_irq_error(uint32_t base_address);

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
 * \brief Get droop mitigation telemetry. Requires debug to be enabled before
 * using.
 *
 * \param base_address Base address of the CCSM.
 *
 * \param[out] telemetry1 Contents of telemetry register 1.
 *
 * \param[out] telemetry2 Contents of telemetry register 2.
 *
 * \param[out] telemetry3 Contents of telemetry register 3.
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
 * \brief Update the modulator/OC state machine config.
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
