/*
 * Arm SCP/MCP Software
 * Copyright (c) 2020-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <mod_scmi_power_domain.h>

#include <fwk_element.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

#include <fwk_dt_config_dyn.h>
#include <fwk_dt_config_common.h>

#ifdef BUILD_HAS_MOD_DEBUG

#define SCMI_PD_COMPAT scp_scmi_pwr_domain

struct fwk_module_config config_dt_scmi_power_domain = {
    .data = &((struct mod_scmi_pd_config) {
        /* ids ordered indexes 0 - 1 */
        .debug_pd_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(SCMI_PD_COMPAT, 0, 0),
        .debug_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(SCMI_PD_COMPAT, 0, 1),
    }),
};

/* cannot support table mods at runtime via the get elements generator function
 * or more than one instance
 */

#ifdef FWK_MODULE_GEN_DYNAMIC_SCMI_POWER_DOMAIN
    #error ("SCMI power domain module does not support DT dynamic configuration")
#endif

static_assert(DT_N_INST_scp_scmi_pwr_domain_NUM_OKAY == 1,
              "Error - multiple scmi-pwr-domain defined in device tree - must only be one");

#else
/* No elements, no module configuration data */
struct fwk_module_config config_dt_scmi_power_domain = { 0 };
#endif

