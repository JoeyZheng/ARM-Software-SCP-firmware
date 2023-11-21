/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "scp_mmap.h"
#include "tc3_dvfs.h"
#include "tc3_scmi.h"
#include "tc3_timer.h"

#include <mod_fch_polled.h>
#include <mod_scmi_perf.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

#include <stdint.h>

#define FC_LEVEL_SET_ADDR(PERF_IDX) \
    (SCMI_FAST_CHANNEL_BASE + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LEVEL_SET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX))

#define FC_LIMIT_SET_ADDR(PERF_IDX) \
    (SCMI_FAST_CHANNEL_BASE + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LIMIT_SET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX))

#define FC_LEVEL_GET_ADDR(PERF_IDX) \
    (SCMI_FAST_CHANNEL_BASE + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LEVEL_GET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX))

#define FC_LIMIT_GET_ADDR(PERF_IDX) \
    (SCMI_FAST_CHANNEL_BASE + MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_LIMIT_GET + \
     (MOD_SCMI_PERF_FAST_CHANNEL_OFFSET_TOTAL * PERF_IDX))

/* get the channel offset by itself and add the AP location */
#define FC_LEVEL_SET_AP_ADDR(PERF_IDX) \
    (FC_LEVEL_SET_ADDR(PERF_IDX) - SCP_SCMI_PAYLOAD_NS_A2P_BASE + \
     SCP_ATU_PHY_ADDR_SHARED_NSRAM)

#define FC_LIMIT_SET_AP_ADDR(PERF_IDX) \
    (FC_LIMIT_SET_ADDR(PERF_IDX) - SCP_SCMI_PAYLOAD_NS_A2P_BASE + \
     SCP_ATU_PHY_ADDR_SHARED_NSRAM)

#define FC_LEVEL_GET_AP_ADDR(PERF_IDX) \
    (FC_LEVEL_GET_ADDR(PERF_IDX) - SCP_SCMI_PAYLOAD_NS_A2P_BASE + \
     SCP_ATU_PHY_ADDR_SHARED_NSRAM)

#define FC_LIMIT_GET_AP_ADDR(PERF_IDX) \
    (FC_LIMIT_GET_ADDR(PERF_IDX) - SCP_SCMI_PAYLOAD_NS_A2P_BASE + \
     SCP_ATU_PHY_ADDR_SHARED_NSRAM)

static struct mod_fch_polled_config module_config = {
    .fch_alarm_id = FWK_ID_SUB_ELEMENT_INIT(
        FWK_MODULE_IDX_TIMER,
        0,
        TC3_CONFIG_TIMER_FAST_CHANNEL_TIMER_IDX),
    .fch_poll_rate = FCH_MIN_POLL_RATE_US,
    .rate_limit = (4 * 1000),
    .attributes = 0,
};

enum fch_polled_length {
    FCH_POLLED_LEVEL_SET_LENGTH = sizeof(uint32_t),
    FCH_POLLED_LIMIT_SET_LENGTH =
        sizeof(struct mod_scmi_perf_fast_channel_limit),
    FCH_POLLED_LEVEL_GET_LENGTH = sizeof(uint32_t),
    FCH_POLLED_LIMIT_GET_LENGTH =
        sizeof(struct mod_scmi_perf_fast_channel_limit)
};

#define FCH_ADDR_INIT(scp_addr, ap_addr, len) \
    &((struct mod_fch_polled_channel_config){ \
        .fch_addr = { \
            .local_view_address = scp_addr, \
            .target_view_address = ap_addr, \
            .length = len, \
        } })

static const struct fwk_element fch_polled_element_table[] = {
    [TC3_PLAT_FCH_CORTEX_A520_LEVEL_SET] = {
        .name = "FCH_CORTEX_A520_LEVEL_SET",
        .data = FCH_ADDR_INIT(
            FC_LEVEL_SET_ADDR(DVFS_ELEMENT_IDX_CORTEX_A520),
            FC_LEVEL_SET_AP_ADDR(DVFS_ELEMENT_IDX_CORTEX_A520),
            FCH_POLLED_LEVEL_SET_LENGTH
            )
    },
    [TC3_PLAT_FCH_CORTEX_A520_LIMIT_SET] = {
        .name = "FCH_CORTEX_A520_LIMIT_SET",
        .data =FCH_ADDR_INIT(
            FC_LIMIT_SET_ADDR(DVFS_ELEMENT_IDX_CORTEX_A520),
            FC_LIMIT_SET_AP_ADDR(DVFS_ELEMENT_IDX_CORTEX_A520),
            FCH_POLLED_LIMIT_SET_LENGTH
            ),
    },
    [TC3_PLAT_FCH_CORTEX_A520_LEVEL_GET] = {
        .name = "FCH_CORTEX_A520_LEVEL_GET",
        .data = FCH_ADDR_INIT(
            FC_LEVEL_GET_ADDR(DVFS_ELEMENT_IDX_CORTEX_A520),
            FC_LEVEL_GET_AP_ADDR(DVFS_ELEMENT_IDX_CORTEX_A520),
            FCH_POLLED_LEVEL_GET_LENGTH
            )
    },
    [TC3_PLAT_FCH_CORTEX_A520_LIMIT_GET] = {
        .name = "FCH_CORTEX_A520_LIMIT_GET",
        .data = FCH_ADDR_INIT(
            FC_LIMIT_GET_ADDR(DVFS_ELEMENT_IDX_CORTEX_A520),
            FC_LIMIT_GET_AP_ADDR(DVFS_ELEMENT_IDX_CORTEX_A520),
            FCH_POLLED_LIMIT_GET_LENGTH
            )
    },
    [TC3_PLAT_FCH_CHABERTON_LEVEL_SET] = {
        .name = "FCH_CHABERTON_LEVEL_SET",
        .data = FCH_ADDR_INIT(
            FC_LEVEL_SET_ADDR(DVFS_ELEMENT_IDX_CHABERTON),
            FC_LEVEL_SET_AP_ADDR(DVFS_ELEMENT_IDX_CHABERTON),
            FCH_POLLED_LEVEL_SET_LENGTH
            )
    },
    [TC3_PLAT_FCH_CHABERTON_LIMIT_SET] = {
        .name = "FCH_CHABERTON_LIMIT_SET",
        .data =FCH_ADDR_INIT(
            FC_LIMIT_SET_ADDR(DVFS_ELEMENT_IDX_CHABERTON),
            FC_LIMIT_SET_AP_ADDR(DVFS_ELEMENT_IDX_CHABERTON),
            FCH_POLLED_LIMIT_SET_LENGTH
            ),
    },
    [TC3_PLAT_FCH_CHABERTON_LEVEL_GET] = {
        .name = "FCH_CHABERTON_LEVEL_GET",
        .data = FCH_ADDR_INIT(
            FC_LEVEL_GET_ADDR(DVFS_ELEMENT_IDX_CHABERTON),
            FC_LEVEL_GET_AP_ADDR(DVFS_ELEMENT_IDX_CHABERTON),
            FCH_POLLED_LEVEL_GET_LENGTH
            )
    },
    [TC3_PLAT_FCH_CHABERTON_LIMIT_GET] = {
        .name = "FCH_CHABERTON_LIMIT_GET",
        .data = FCH_ADDR_INIT(
            FC_LIMIT_GET_ADDR(DVFS_ELEMENT_IDX_CHABERTON),
            FC_LIMIT_GET_AP_ADDR(DVFS_ELEMENT_IDX_CHABERTON),
            FCH_POLLED_LIMIT_GET_LENGTH
            )
    },
    [TC3_PLAT_FCH_BLACKHAWK_LEVEL_SET] = {
        .name = "FCH_BLACKHAWK_LEVEL_SET",
        .data = FCH_ADDR_INIT(
            FC_LEVEL_SET_ADDR(DVFS_ELEMENT_IDX_BLACKHAWK),
            FC_LEVEL_SET_AP_ADDR(DVFS_ELEMENT_IDX_BLACKHAWK),
            FCH_POLLED_LEVEL_SET_LENGTH
            )
    },
    [TC3_PLAT_FCH_BLACKHAWK_LIMIT_SET] = {
        .name = "FCH_BLACKHAWK_LIMIT_SET",
        .data =FCH_ADDR_INIT(
            FC_LIMIT_SET_ADDR(DVFS_ELEMENT_IDX_BLACKHAWK),
            FC_LIMIT_SET_AP_ADDR(DVFS_ELEMENT_IDX_BLACKHAWK),
            FCH_POLLED_LIMIT_SET_LENGTH
            ),
    },
    [TC3_PLAT_FCH_BLACKHAWK_LEVEL_GET] = {
        .name = "FCH_BLACKHAWK_LEVEL_GET",
        .data = FCH_ADDR_INIT(
            FC_LEVEL_GET_ADDR(DVFS_ELEMENT_IDX_BLACKHAWK),
            FC_LEVEL_GET_AP_ADDR(DVFS_ELEMENT_IDX_BLACKHAWK),
            FCH_POLLED_LEVEL_GET_LENGTH
            )
    },
    [TC3_PLAT_FCH_BLACKHAWK_LIMIT_GET] = {
        .name = "FCH_BLACKHAWK_LIMIT_GET",
        .data = FCH_ADDR_INIT(
            FC_LIMIT_GET_ADDR(DVFS_ELEMENT_IDX_BLACKHAWK),
            FC_LIMIT_GET_AP_ADDR(DVFS_ELEMENT_IDX_BLACKHAWK),
            FCH_POLLED_LIMIT_GET_LENGTH
            )
    },
    [TC3_PLAT_FCH_GPU_LEVEL_SET] = {
        .name = "FCH_GPU_LEVEL_SET",
        .data = FCH_ADDR_INIT(
            FC_LEVEL_SET_ADDR(DVFS_ELEMENT_IDX_GPU),
            FC_LEVEL_SET_AP_ADDR(DVFS_ELEMENT_IDX_GPU),
            FCH_POLLED_LEVEL_SET_LENGTH
            )
    },
    [TC3_PLAT_FCH_GPU_LIMIT_SET] = {
        .name = "FCH_GPU_LIMIT_SET",
        .data =FCH_ADDR_INIT(
            FC_LIMIT_SET_ADDR(DVFS_ELEMENT_IDX_GPU),
            FC_LIMIT_SET_AP_ADDR(DVFS_ELEMENT_IDX_GPU),
            FCH_POLLED_LIMIT_SET_LENGTH
            ),
    },
    [TC3_PLAT_FCH_GPU_LEVEL_GET] = {
        .name = "FCH_GPU_LEVEL_GET",
        .data = FCH_ADDR_INIT(
            FC_LEVEL_GET_ADDR(DVFS_ELEMENT_IDX_GPU),
            FC_LEVEL_GET_AP_ADDR(DVFS_ELEMENT_IDX_GPU),
            FCH_POLLED_LEVEL_GET_LENGTH
            )
    },
    [TC3_PLAT_FCH_GPU_LIMIT_GET] = {
        .name = "FCH_GPU_LIMIT_GET",
        .data = FCH_ADDR_INIT(
            FC_LIMIT_GET_ADDR(DVFS_ELEMENT_IDX_GPU),
            FC_LIMIT_GET_AP_ADDR(DVFS_ELEMENT_IDX_GPU),
            FCH_POLLED_LIMIT_GET_LENGTH
            )
    },

    [TC3_PLAT_FCH_COUNT] = {0},
};

static const struct fwk_element *fch_polled_get_element_table(
    fwk_id_t module_id)
{
    return fch_polled_element_table;
}

const struct fwk_module_config config_fch_polled = {
    .data = &module_config,
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(fch_polled_get_element_table),
};
