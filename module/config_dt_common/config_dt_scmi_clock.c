/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <mod_scmi_clock.h>

#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

#include <fwk_dt_config_common.h>

#if 0

/* SCMI clocks controlled by OSPM */
            ospm_clock_table: scmi-clock-table {
                compatible = "scp,scmi-clock-table";

                big_clk {
                    scp-ids = <&elem_id FWK_MODULE_IDX_CLOCK JUNO_CLOCK_IDX_BIGCLK>;
                    starts-enabled;
                };

agent_ospm: scmi-agent@1 {
                compatible = "scp,scmi-agent";
                label = "OSPM";
                agent-type = <SCMI_AGENT_TYPE_OSPM>;
                agent-idx = <JUNO_SCMI_AGENT_IDX_OSPM>;
                scmi-clock-table = <&ospm_clock_table>;                     
            };



static const struct mod_scmi_clock_device agent_device_table_ospm[] = {
    {
        /* Big */
        .element_id =
            FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_CLOCK, JUNO_CLOCK_IDX_BIGCLK),
        .starts_enabled = true,
    },
    {
        /* Little */
        .element_id =
            FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_CLOCK, JUNO_CLOCK_IDX_LITTLECLK),
        .starts_enabled = true,
    },
    {
        /* GPU */
        .element_id =
            FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_CLOCK, JUNO_CLOCK_IDX_GPUCLK),
        .starts_enabled = true,
    },
    {
        /* HDLCD0 */
        .element_id =
            FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_CLOCK, JUNO_CLOCK_IDX_HDLCD0),
        .starts_enabled = true,
    },
    {
        /* HDLCD1 */
        .element_id =
            FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_CLOCK, JUNO_CLOCK_IDX_HDLCD1),
        .starts_enabled = true,
    },
    {
        /* I2S */
        .element_id =
            FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_CLOCK, JUNO_CLOCK_IDX_I2SCLK),
        .starts_enabled = true,
    },
};
#endif

/* generate clock tables used by scmi agents */
#define SCT_COMPAT scp_scmi_clock_table
#define SCT_NODE_ID(n) DT_INST(n, SCT_COMPAT)
#define SCT_TABLE_NAME(n) DT_STRING_TOKEN(SCT_NODE_ID(n), label)

/* extract the clock index parameter from the scp-ids triple */
#define SCT_SCP_ID_CLK_IDX(node_id)  DT_PROP_BY_IDX(child_node_id, scp_ids, 2)

#define SCT_GEN_CLOCK_TABLE_ELEMENT(child_node_id) \
    { \
        .starts_enabled = DT_PROP_OR(child_node_id, starts_enabled, 0), \
        .element_id =  FWK_DT_NODE_PH_IDX_SCP_ID_ELEM(child_node_id, 0),  \
    },

#define SCMI_GEN_NAMED_RESET_TABLE(n) \
static const struct mod_scmi_clock_device SCT_TABLE_NAME(n)[] = { \
            DT_FOREACH_CHILD(SCT_NODE_ID(n), SCT_GEN_CLOCK_TABLE_ELEMENT) \
};

DT_FOREACH_OKAY_INST_scp_scmi_reset_devices(SCMI_GEN_NAMED_RESET_TABLE)


/* generate an agent table with references to clock tables */
#define SCMI_AT_COMPAT scp_scmi_agent
#define SCMI_AT_PROP(n, prop) FWK_DT_INST_PROP(SCMI_AT_COMPAT, n , prop)

#define PH_TABLE_NAME(ph_node_id) DT_STRING_TOKEN(ph_node_id, label)

#define SCMI_CLOCK_AGENT_TABLE_ENTRY(ph_node_id) \
        .device_table = PH_TABLE_NAME(ph_node_id), \
        .device_count = FWK_ARRAY_SIZE(PH_TABLE_NAME(ph_node_id)),

// if agent->scmi-clock-table then set phandle name to link to generated table else { 0 },
#define SCMI_AGENT_CLOCK_TABLE_REF_OR_NONE(n)  \
    COND_CODE_1(DT_NODE_HAS_PROP(DT_INST(n , SCMI_AT_COMPAT), scmi_clock_table), \
        (SCMI_CLOCK_AGENT_TABLE_ENTRY(DT_PHANDLE(DT_INST(n, SCMI_AT_COMPAT), scmi_clock_table))), \
        ( 0 ) \
    )

// each table entry will have a link to a clock table if declared in the DT.
#define SCMI_AT_INIT(n) \
    [SCMI_AT_PROP(n, agent_idx)] = { \
        SCMI_AGENT_CLOCK_TABLE_REF_OR_NONE(n) \
    },

/* iterate through the scmi defined DT agent table and generate an agent table array
 * here, with entries referencing clock tables if present in DT
*/
static const struct mod_scmi_clock_agent agent_table[] = {
    DT_FOREACH_OKAY_INST_scp_scmi_agent(SCMI_AT_INIT)    
};

#if 0
static const struct
    mod_scmi_clock_agent agent_table[JUNO_SCMI_SERVICE_IDX_COUNT] = {
    [JUNO_SCMI_AGENT_IDX_PSCI] = { 0 /* No Access */ },
    [JUNO_SCMI_AGENT_IDX_OSPM] = {
        .device_table = agent_device_table_ospm,
        .device_count = FWK_ARRAY_SIZE(agent_device_table_ospm),
    },
};
#endif

struct fwk_module_config config_dt_scmi_clock = {
    .data = &((struct mod_scmi_clock_config) {
        .max_pending_transactions = 0,
        .agent_table = agent_table,
        .agent_count = FWK_ARRAY_SIZE(agent_table),
    }),
};


#ifdef FWK_MODULE_GEN_DYNAMIC_SCMI_CLOCK
#error ("module scmi-clock cannot use the DT dynamic configuration")
#endif