/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "clock_soc.h"
#include "config_power_domain.h"
#include "tc_core.h"

#include <mod_clock.h>
#include <mod_css_clock.h>
#include <mod_pik_clock.h>
#include <mod_power_domain.h>
#include <mod_tc3_bl1.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

static const struct fwk_element clock_dev_desc_table[2] = {
    [CLOCK_IDX_CPU_GROUP_GROUP_LITTLE] =
        {
            .name = "CPU_GROUP_" TC_GROUP_LITTLE_NAME,
            .data = &((struct mod_clock_dev_config){
                .driver_id = FWK_ID_ELEMENT_INIT(
                    FWK_MODULE_IDX_CSS_CLOCK,
                    CLOCK_CSS_IDX_CPU_GROUP_GROUP_LITTLE),
                .api_id = FWK_ID_API_INIT(
                    FWK_MODULE_IDX_CSS_CLOCK,
                    MOD_CSS_CLOCK_API_TYPE_CLOCK),
                .pd_source_id = FWK_ID_MODULE_INIT(FWK_MODULE_IDX_TC3_BL1),
            }),
        },
    { 0 }, /* Termination description. */
};

static const struct fwk_element *clock_get_dev_desc_table(fwk_id_t module_id)
{
    return clock_dev_desc_table;
}

const struct fwk_module_config config_clock = {
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(clock_get_dev_desc_table),
    .data = &((struct mod_clock_config){
        .pd_transition_notification_id = FWK_ID_NOTIFICATION_INIT(
            FWK_MODULE_IDX_TC3_BL1,
            MOD_TC3_BL1_NOTIFICATION_IDX_POWER_SYSTOP),
        .pd_pre_transition_notification_id = FWK_ID_NONE_INIT,
    }),
};
