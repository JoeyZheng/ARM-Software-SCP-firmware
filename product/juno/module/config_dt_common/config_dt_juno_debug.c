/*
 * Arm SCP/MCP Software
 * Copyright (c) 2020-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <config_power_domain.h>
#include <juno_id.h>
#include <system_clock.h>

#include <mod_debug.h>
#include <mod_juno_debug.h>

#include <fwk_element.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

#include <fwk_dt_config_dyn.h>
#include <fwk_dt_config_common.h>


#define J_DBG_COMPAT arm_juno_debug_scp_mod
#define J_DBG_PROP(n, prop) FWK_DT_INST_PROP(J_DBG_COMPAT, n, prop)
#define J_DBG_CLK_DIV(n, prop) (SYSINCLK / ((unsigned long)J_DBG_PROP(n, prop) * FWK_MHZ))

/* Debug clock settings for Juno with default values for R0 */
static struct juno_css_debug_dev clock_settings = {
    .div_atclk = J_DBG_CLK_DIV(0, arm_atclk_div_mhz),
    .manual_reset_required = true,
    .div_pclk = J_DBG_CLK_DIV(0, arm_pclk_div),
    .div_traceclk = J_DBG_CLK_DIV(0, arm_trace_clk_div_mhz),
};

/* only a single debug module can be defines - instance 0 index */
static struct fwk_element juno_debug_element_table[] = {
    [0] = {
        .name = J_DBG_PROP(0, label),
        .data = &((struct mod_juno_debug_dev_config){
            /* IDs in DT are ordered list - indexes 1 - 3 */
            .pd_dbgsys_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(J_DBG_COMPAT, 0, 1),                
            .pd_big_sstop_id =  FWK_DT_PH_IDX_SCP_ID_BY_TYPE(J_DBG_COMPAT, 0, 2),
            .pd_little_sstop_id =  FWK_DT_PH_IDX_SCP_ID_BY_TYPE(J_DBG_COMPAT, 0, 3),
        }),
    },
    [1] = { 0 }, /* Termination description */
};

#ifdef FWK_MODULE_GEN_DYNAMIC_JUNO_DEBUG
const struct fwk_element *_static_get_element_table_juno_debug() {
    return juno_debug_element_table;
};

const struct fwk_element *dyn_get_element_table_juno_debug(fwk_id_t module_id)
{
    int status;
    enum juno_idx_revision revision;
    struct mod_juno_debug_dev_config *juno_debug_config;

    status = juno_id_get_revision(&revision);
    if (status != FWK_SUCCESS) {
        return NULL;
    }

    juno_debug_config = (struct mod_juno_debug_dev_config *)
        juno_debug_element_table[0].data;

    juno_debug_config->clk_settings = &clock_settings;

    /* Adjust Debug clock settings for Juno R1 & Juno R2 */
    if (revision != JUNO_IDX_REVISION_R0) {
        juno_debug_config->clk_settings->div_atclk = J_DBG_CLK_DIV(0, arm_atclk_div_mhz_r1r2),
        juno_debug_config->clk_settings->manual_reset_required = false;
    }

    return juno_debug_element_table;
}

struct fwk_module_config config_dt_juno_debug = {
    .data =
        &(struct mod_juno_debug_config){
            .timer_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(J_DBG_COMPAT, 0, 0),
        },

    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dyn_get_element_table_juno_debug),
};
#else
#error("Error: module juno_debug must use dynamic dts module access to override values");
#endif