/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019-2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* device tree configuration for statistics module */


#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>

#include <fwk_devicetree.h>
#include <fwk_dt_config_common.h>

#include <mod_stats.h>


/* DT macros */
#define ST_COMPAT scp_statistics
#define ST_NODE_ID DT_INST(0, ST_COMPAT)

static const struct mod_stats_config_info stats_config_info = {
    .ap_stats_addr = DT_REG_ADDR_BY_IDX(ST_NODE_ID, 1),
    .scp_stats_addr = DT_REG_ADDR_BY_IDX(ST_NODE_ID, 0),
    .stats_region_size = DT_REG_SIZE_BY_IDX(ST_NODE_ID, 0),
    .alarm_id = FWK_DT_PH_IDX_SCP_ID_SUB_ELEM(ST_COMPAT, 0, 0),
};

struct fwk_module_config config_dt_statistics = {
    .data = &stats_config_info,
};

/* single static instance of this element */
static_assert(DT_N_INST_scp_scmi_perf_NUM_OKAY == 1,
              "Error - multiple scp-statistics instances defined in device tree - must only be one");

#ifdef FWK_MODULE_GEN_DYNAMIC_SCMI_PERF
#error ("module scmi-perf cannot use the DT dynamic configuration")
#endif

