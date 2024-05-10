/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

#include <fwk_dt_config_common.h>

#include <mod_scmi_reset_domain.h>

/* construct scmi reset device tables (SRDT) here - linked to agent tables by name 

DT excerpt: 

ospm_reset_table: scmi-reset-devices {
                compatible = "scp,scmi-reset-devices";
                label = "ospm_agent_reset_table";

                uart {
                        elem_idx = <JUNO_RESET_DOMAIN_IDX_UART>;
                        scp-ids = <&elem_id FWK_MODULE_IDX_RESET_DOMAIN JUNO_RESET_DOMAIN_IDX_UART>;
                        reset-perms = <MOD_SCMI_RESET_DOMAIN_PERM_ATTRIBUTES | MOD_SCMI_RESET_DOMAIN_PERM_RESET>;
                };
            };


generates: 

static const struct mod_scmi_reset_domain_device ospm_agent_reset_table[] = {
   [JUNO_RESET_DOMAIN_IDX_UART] = {
        .element_id =
            FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_RESET_DOMAIN,
                                JUNO_RESET_DOMAIN_IDX_UART),
        .permissions = MOD_SCMI_RESET_DOMAIN_PERM_ATTRIBUTES |
                       MOD_SCMI_RESET_DOMAIN_PERM_RESET,
    },
};

*/
#define SRDT_COMPAT scp_scmi_reset_devices
#define SRDT_NODE_ID(n) DT_INST(n, SRDT_COMPAT)
#define SRDT_TABLE_NAME(n) DT_STRING_TOKEN(SRDT_NODE_ID(n), label)

#define SRDT_GEN_RESET_ELEMENT(child_node_id) \
    [DT_PROP(child_node_id, elem_idx)] = { \
        .permissions = DT_PROP(child_node_id, reset_perms), \
        .element_id =  FWK_DT_NODE_PH_IDX_SCP_ID_ELEM(child_node_id, 0),  \
    },


#define SCMI_GEN_NAMED_RESET_TABLE(n) \
static const struct mod_scmi_reset_domain_device SRDT_TABLE_NAME(n)[] = { \
            DT_FOREACH_CHILD(SRDT_NODE_ID(n), SRDT_GEN_RESET_ELEMENT) \
};

DT_FOREACH_OKAY_INST_scp_scmi_reset_devices(SCMI_GEN_NAMED_RESET_TABLE)

/* Generate an agent table of all delacred agents and links to reset device tables 

DT excerpt: 

agent_psci: scmi-agent@0 {
    compatible = "scp,scmi-agent";
    label = "PCSI";
    ...
    agent-idx = <JUNO_SCMI_AGENT_IDX_PSCI>;
};

agent_ospm: scmi-agent@1 {
    compatible = "scp,scmi-agent";
    label = "OSPM";
    ...
    agent-idx = <JUNO_SCMI_AGENT_IDX_OSPM>;
};

&agent_ospm {
    scmi-reset-table = <&ospm_reset_table>;
};


generates:

static const struct mod_scmi_reset_domain_agent agent_table[] = {
    [JUNO_SCMI_AGENT_IDX_PSCI] = { 0 },
    [JUNO_SCMI_AGENT_IDX_OSPM] = {
        .device_table = ospm_agent_reset_table,
        .agent_domain_count = FWK_ARRAY_SIZE(ospm_agent_reset_table),
    },
};

*/

#define SCMI_AT_COMPAT scp_scmi_agent
#define SCMI_AT_PROP(n, prop) FWK_DT_INST_PROP(SCMI_AT_COMPAT, n , prop)

#define PH_TABLE_NAME(ph_node_id) DT_STRING_TOKEN(ph_node_id, label)
#define SCMI_RESET_TABLE_ENTRY(ph_node_id) \
        .device_table = PH_TABLE_NAME(ph_node_id), \
        .agent_domain_count = FWK_ARRAY_SIZE(PH_TABLE_NAME(ph_node_id)),

// if agent->scmi-reset-table then set phandle name to link to generated table else { 0 },
#define SCMI_RESET_TABLE_REF_OR_NONE(n)  \
    COND_CODE_1(DT_NODE_HAS_PROP(DT_INST(n , SCMI_AT_COMPAT), scmi_reset_table), \
        (SCMI_RESET_TABLE_ENTRY(DT_PHANDLE(DT_INST(n, SCMI_AT_COMPAT), scmi_reset_table))), \
        ( 0 ) \
    )

// each table entry will have a link to a reset device table if declared in the DT.
#define SCMI_AT_INIT(n) \
    [SCMI_AT_PROP(n, agent_idx)] = { \
        SCMI_RESET_TABLE_REF_OR_NONE(n) \
    },

/* iterate through the scmi defined DT agent table and generate an agent table array
 * here, with entries referencing reset element tables if present in DT
*/
static const struct mod_scmi_reset_domain_agent agent_table[] = {
    DT_FOREACH_OKAY_INST_scp_scmi_agent(SCMI_AT_INIT)    
};

/* main module config - link to generated agent_table[] */
struct fwk_module_config config_dt_scmi_reset_domain = {
    .data = &((struct mod_scmi_reset_domain_config) {
        .agent_table = agent_table,
        .agent_count = FWK_ARRAY_SIZE(agent_table),
    }),
};

#ifdef FWK_MODULE_GEN_DYNAMIC_SCMI_RESET_DOMAIN
#error ("module scmi-reset-domain cannot use the DT dynamic configuration")
#endif