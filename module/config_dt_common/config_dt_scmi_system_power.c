/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mod_scmi_system_power.h>

#include <fwk_module.h>
#include <fwk_devicetree.h>
#include <fwk_dt_config_common.h>



#define SCMI_SYS_PWR_COMPAT scp_scmi_system_power
#define SCMI_SYS_PWR_PROP(prop) FWK_DT_INST_PROP(SCMI_SYS_PWR_COMPAT, 0, prop)
#define SCMI_SYS_PWR_PROP_OR_DEF(prop, def) FWK_DT_INST_PROP_DEFAULT(SCMI_SYS_PWR_COMPAT, 0, prop, def)
#define SCMI_SYS_ID_NAME_ALARM alarm
#define SCMI_SYS_ID_NAME_WAKEUP wakeup
#define SCMI_SYS_ID_BY_NAME(name) \
    FWK_DT_PH_IDX_SCP_ID_BY_NAME(SCMI_SYS_PWR_COMPAT, 0, name)
#define SCMI_SYS_ID_BY_NAME_OR_NONE(name) \
    FWK_DT_PH_IDX_SCP_ID_BY_NAME_OR_NONE(SCMI_SYS_PWR_COMPAT, 0, name)


#ifdef DEBUG_DT_MACROS
static int alarm_idx = FWK_DT_PH_SCP_ID_NAME_TO_IDX(SCMI_SYS_PWR_COMPAT, 0, alarm);
static int alarm_nodeid = FWK_DT_PH_SCP_ID_NAME_NODE_ID(SCMI_SYS_PWR_COMPAT, 0, alarm);
static int alarm_name_exists = FWK_DT_PH_SCP_ID_NAME_EXISTS(SCMI_SYS_PWR_COMPAT, 0, alarm);
static int alarm_idx_alt = DT_PHA_IDX_OF_NAME(DT_INST(0, SCMI_SYS_PWR_COMPAT),scp_ids, alarm);
static int alarm_idx_alt_2 = FWK_DT_PH_NAME_HAS_IDX(SCMI_SYS_PWR_COMPAT, 0, scp_ids, alarm);
static int alarm_name_exists_2 = FWK_DT_PH_NAME_EXISTS(SCMI_SYS_PWR_COMPAT, 0, scp_ids, alarm);
static int bob_name_exists = FWK_DT_PH_NAME_EXISTS(SCMI_SYS_PWR_COMPAT, 0, scp_ids, bob);
static int wakeup_name_exists = FWK_DT_PH_NAME_EXISTS(SCMI_SYS_PWR_COMPAT, 0, scp_ids, wakeup);
static int wakeup_idx = FWK_DT_PH_NAME_HAS_IDX(SCMI_SYS_PWR_COMPAT, 0, scp_ids, wakeup);

static int test1 = DT_PHANDLE_BY_IDX(DT_INST(0, SCMI_SYS_PWR_COMPAT), scp_ids, FWK_DT_PH_NAME_HAS_IDX(SCMI_SYS_PWR_COMPAT, 0, scp_ids, wakeup));

static fwk_id id_idx = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(SCMI_SYS_PWR_COMPAT, 0, 1);
static fwk_id id_name = FWK_DT_PH_IDX_SCP_ID_BY_NAME(SCMI_SYS_PWR_COMPAT, 0, wakeup);
static fwk_id id_name = FWK_DT_PH_IDX_SCP_ID_BY_NAME_X(SCMI_SYS_PWR_COMPAT, 0, wakeup);
#endif


struct fwk_module_config config_dt_scmi_system_power = {
    .data =
        &(struct mod_scmi_system_power_config){
            .wakeup_power_domain_id = SCMI_SYS_ID_BY_NAME_OR_NONE(SCMI_SYS_ID_NAME_WAKEUP),
            .wakeup_composite_state = SCMI_SYS_PWR_PROP_OR_DEF(wakeup_state, 0),
            .system_view = SCMI_SYS_PWR_PROP(system_view),
            .system_suspend_state = (unsigned int)SCMI_SYS_PWR_PROP(system_suspend),
            .alarm_id = SCMI_SYS_ID_BY_NAME(SCMI_SYS_ID_NAME_ALARM),
            .graceful_timeout = SCMI_SYS_PWR_PROP(timeout_us),
        },
};

/* ensure single instance and static declaration*/
static_assert(DT_N_INST_scp_scmi_system_power_NUM_OKAY == 1,
              "Error - multiple scmi-system-power instances defined in device tree - must only be one");

#ifdef FWK_MODULE_GEN_DYNAMIC_SCMI_SYSTEM_POWER
#error ("module scmi-system-power cannot use the DT dynamic configuration")
#endif

/*
struct fwk_module_config config_scmi_system_power = {
    .data =
        &(struct mod_scmi_system_power_config){
            .system_view = MOD_SCMI_SYSTEM_VIEW_FULL,
            .system_suspend_state =
                (unsigned int)MOD_SYSTEM_POWER_POWER_STATE_SLEEP0,
            .alarm_id = FWK_ID_SUB_ELEMENT_INIT(
                FWK_MODULE_IDX_TIMER,
                JUNO_ALARM_ELEMENT_IDX,
                JUNO_SYSTEM_POWER_ALARM_IDX),
            .graceful_timeout = 1000, 
        },
};*/
