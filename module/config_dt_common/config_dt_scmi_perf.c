/*
 * Arm SCP/MCP Software
 * Copyright (c) 2020-2023, Linaro Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "config_dvfs.h"
#include "juno_alarm_idx.h"
#include "juno_scmi.h"
#include "software_mmap.h"

#include <internal/scmi_perf.h>
#include <mod_scmi_perf.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

#include <fwk_devicetree.h>
#include <fwk_dt_config_common.h>

#include <stddef.h>
#include <stdint.h>

#ifdef BUILD_HAS_MOD_STATISTICS
#    include <mod_stats.h>
#endif

/* macros for accessing device tree parameters */
#define SCMI_PERF_COMPAT scp_scmi_perf
#define SCMI_PERF_PROP(prop) FWK_DT_INST_PROP(SCMI_PERF_COMPAT, 0, prop)
#define SCMI_PERF_NODE_ID DT_INST(0, SCMI_PERF_COMPAT)

/* base address is first entry in reg property */
#define SCMI_FC_BASE_DT DT_REG_ADDR_BY_IDX(SCMI_PERF_NODE_ID, 0)

#define FC_LEVEL_SET(PERF_IDX) \
    (SCMI_FC_BASE_DT + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LEVEL_SET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX))

#define FC_LIMIT_SET(PERF_IDX) \
    (SCMI_FC_BASE_DT + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LIMIT_SET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX))

#define FC_LEVEL_GET(PERF_IDX) \
    (SCMI_FC_BASE_DT + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LEVEL_GET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX))

#define FC_LIMIT_GET(PERF_IDX) \
    (SCMI_FC_BASE_DT + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LIMIT_GET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX))

#define FC_LEVEL_SET_AP(PERF_IDX) \
    (SCMI_FC_BASE_DT + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LEVEL_SET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX) - EXTERNAL_DEV_BASE)

#define FC_LIMIT_SET_AP(PERF_IDX) \
    (SCMI_FC_BASE_DT + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LIMIT_SET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX) - EXTERNAL_DEV_BASE)

#define FC_LEVEL_GET_AP(PERF_IDX) \
    (SCMI_FC_BASE_DT + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LEVEL_GET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX) - EXTERNAL_DEV_BASE)

#define FC_LIMIT_GET_AP(PERF_IDX) \
    (SCMI_FC_BASE_DT + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LIMIT_GET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX) - EXTERNAL_DEV_BASE)

/* build options affecting the defines */
#ifdef BUILD_HAS_MOD_STATISTICS
    #define STATS_COLLECTED 1
#else
    #define STATS_COLLECTED 0
#endif

#ifdef BUILD_HAS_SCMI_PERF_FAST_CHANNELS
    #define FC_ADDR_SCP_ELEM(perf_idx) \
        .fast_channels_addr_scp = (uint64_t[]) { \
            [MOD_SCMI_PERF_FAST_CHANNEL_LEVEL_SET] = \
                FC_LEVEL_SET(perf_idx), \
            [MOD_SCMI_PERF_FAST_CHANNEL_LIMIT_SET] = \
                FC_LIMIT_SET(perf_idx), \
            [MOD_SCMI_PERF_FAST_CHANNEL_LEVEL_GET] = \
                FC_LEVEL_GET(perf_idx), \
            [MOD_SCMI_PERF_FAST_CHANNEL_LIMIT_GET] = \
                FC_LIMIT_GET(perf_idx), \
        },
    #define FC_ADDR_AP_ELEM(perf_idx) \
        .fast_channels_addr_ap = (uint64_t[]) { \
            [MOD_SCMI_PERF_FAST_CHANNEL_LEVEL_SET] = \
                FC_LEVEL_SET_AP(perf_idx), \
            [MOD_SCMI_PERF_FAST_CHANNEL_LIMIT_SET] = \
                FC_LIMIT_SET_AP(perf_idx), \
            [MOD_SCMI_PERF_FAST_CHANNEL_LEVEL_GET] = \
                FC_LEVEL_GET_AP(perf_idx), \
            [MOD_SCMI_PERF_FAST_CHANNEL_LIMIT_GET] = \
                FC_LIMIT_GET_AP(perf_idx), \
        },
#else
    #define FC_ADDR_SCP_ELEM(perf_idx) \
        .fast_channels_addr_scp = 0,
    #define FC_ADDR_AP_ELEM(perf_idx) \
        .fast_channels_addr_ap = 0,
#endif

#define DOMAIN_ARRAY_ELEM(idx) \
    [idx] = { \
        FC_ADDR_SCP_ELEM(idx) \
        FC_ADDR_AP_ELEM(idx) \
        .stats_collected = STATS_COLLECTED, \
    },

#define DOMAIN_ARRAY_IDX_VAL(node_id, prop, prop_idx) \
    DOMAIN_ARRAY_ELEM(DT_PROP_BY_IDX(node_id, prop, prop_idx))

static const struct mod_scmi_perf_domain_config domains[] = {
    DT_FOREACH_PROP_ELEM(SCMI_PERF_NODE_ID, perf_domain_indexes, DOMAIN_ARRAY_IDX_VAL)
};

struct fwk_module_config config_dt_scmi_perf = {
    .data = &((struct mod_scmi_perf_config){
        .domains = &domains,
        .perf_doms_count = FWK_ARRAY_SIZE(domains),
#ifdef BUILD_HAS_SCMI_PERF_FAST_CHANNELS
        .fast_channels_alarm_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(SCMI_PERF_COMPAT, 0, 0),
        .fast_channels_rate_limit = SCMI_PERF_PROP(fast_chan_rate_limit),
#else
        .fast_channels_alarm_id = FWK_ID_NONE_INIT,
#endif

        .stats_enabled = STATS_COLLECTED,
    }),
};

/* ensure single instance and static declaration*/
static_assert(DT_N_INST_scp_scmi_perf_NUM_OKAY == 1,
              "Error - multiple scmi-perf instances defined in device tree - must only be one");

#ifdef FWK_MODULE_GEN_DYNAMIC_SCMI_PERF
#error ("module scmi-perf cannot use the DT dynamic configuration")
#endif