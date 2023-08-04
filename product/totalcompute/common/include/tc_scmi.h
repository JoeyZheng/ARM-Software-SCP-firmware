/*
 * Arm SCP/MCP Software
 * Copyright (c) 2022-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Definitions for SCMI and TRANSPORT module configurations.
 */

#ifndef SCP_TC_SCMI_H
#define SCP_TC_SCMI_H

/* SCMI agent identifiers */
enum scp_tc_scmi_agent_id {
    /* 0 is reserved for the platform */
    SCP_SCMI_AGENT_ID_OSPM = 1,
    SCP_SCMI_AGENT_ID_PSCI,
    SCP_SCMI_AGENT_ID_COUNT
};

/* SCMI service indexes */
enum scp_tc_scmi_service_idx {
    SCP_TC_SCMI_SERVICE_IDX_PSCI,
    SCP_TC_SCMI_SERVICE_IDX_OSPM_0,
    SCP_TC_SCMI_SERVICE_IDX_OSPM_1,
    SCP_TC_SCMI_SERVICE_IDX_COUNT,
};

/* SCP<->AP Fast channels indexes */
enum tc2_plat_fch_idx {
    /* Fast channels from 0 - 3 */
    TC2_PLAT_FCH_CORTEX_A520_LEVEL_SET,
    TC2_PLAT_FCH_CORTEX_A520_LIMIT_SET,
    TC2_PLAT_FCH_CORTEX_A520_LEVEL_GET,
    TC2_PLAT_FCH_CORTEX_A520_LIMIT_GET,

    /* Fast channels from 4 - 7 */
    TC2_PLAT_FCH_CORTEX_A720_LEVEL_SET,
    TC2_PLAT_FCH_CORTEX_A720_LIMIT_SET,
    TC2_PLAT_FCH_CORTEX_A720_LEVEL_GET,
    TC2_PLAT_FCH_CORTEX_A720_LIMIT_GET,

    /* Fast channels from 8 - 11 */
    TC2_PLAT_FCH_CORTEX_X4_LEVEL_SET,
    TC2_PLAT_FCH_CORTEX_X4_LIMIT_SET,
    TC2_PLAT_FCH_CORTEX_X4_LEVEL_GET,
    TC2_PLAT_FCH_CORTEX_X4_LIMIT_GET,

    /* Fast chanels from 12 - 15*/
    TC2_PLAT_FCH_GPU_LEVEL_SET,
    TC2_PLAT_FCH_GPU_LIMIT_SET,
    TC2_PLAT_FCH_GPU_LEVEL_GET,
    TC2_PLAT_FCH_GPU_LIMIT_GET,

    TC2_PLAT_FCH_COUNT,
};

/* SCP <-> AP Transport channels */
enum tc2_transport_channel_idx {
    TC2_TRANSPORT_SCMI_SERVICE_PSCI,
    TC2_TRANSPORT_SCMI_SERVICE_OSPM_0,
    TC2_TRANSPORT_SCMI_SERVICE_OSPM_1,

#if defined(BUILD_HAS_SCMI_PERF_FAST_CHANNELS)
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_A520_LEVEL_SET,
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_A520_LIMIT_SET,
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_A520_LEVEL_GET,
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_A520_LIMIT_GET,

    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_A720_LEVEL_SET,
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_A720_LIMIT_SET,
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_A720_LEVEL_GET,
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_A720_LIMIT_GET,

    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_X4_LEVEL_SET,
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_X4_LIMIT_SET,
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_X4_LEVEL_GET,
    TC2_TRANSPORT_SCMI_PERF_FCH_CORTEX_X4_LIMIT_GET,

    TC2_TRANSPORT_SCMI_PERF_FCH_GPU_LEVEL_SET,
    TC2_TRANSPORT_SCMI_PERF_FCH_GPU_LIMIT_SET,
    TC2_TRANSPORT_SCMI_PERF_FCH_GPU_LEVEL_GET,
    TC2_TRANSPORT_SCMI_PERF_FCH_GPU_LIMIT_GET,
#endif
    TC2_TRANSPORT_CHANNELS_COUNT,
};

#endif /* SCP_TC_SCMI_H */
