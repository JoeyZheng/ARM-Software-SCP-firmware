/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "config_power_domain.h"
#include "scp_mmap.h"
#include "scp_tc_mhu3.h"
#include "tc3_core.h"
#include "tc3_scmi.h"

#include <mod_fch_polled.h>
#include <mod_transport.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

#include <stdint.h>

static const struct fwk_element transport_element_table[
#ifdef BUILD_HAS_MOD_TRANSPORT_FC
    TC3_TRANSPORT_CHANNELS_COUNT + 1] = {
    [TC3_TRANSPORT_SCMI_SERVICE_PSCI] = {
#else
    SCP_TC3_SCMI_SERVICE_IDX_COUNT + 1] = {
    [SCP_TC3_SCMI_SERVICE_IDX_PSCI] = {
#endif
        .name = "PSCI",
        .data = &((
            struct mod_transport_channel_config){
            .channel_type = MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .policies =
                MOD_TRANSPORT_POLICY_INIT_MAILBOX |
                MOD_TRANSPORT_POLICY_SECURE,
            .out_band_mailbox_address = (uintptr_t)
                SCP_SCMI_PAYLOAD_S_A2P_BASE,
            .out_band_mailbox_size =
                SCP_SCMI_PAYLOAD_SIZE,
            .driver_id = FWK_ID_SUB_ELEMENT_INIT(
                FWK_MODULE_IDX_MHU3,
                SCP_TC_MHU3_DEVICE_IDX_SCP_AP_S,
                0),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_MHU3,
                0),
        }),
     },
#ifdef BUILD_HAS_MOD_TRANSPORT_FC
    [TC3_TRANSPORT_SCMI_SERVICE_OSPM_A2P] = {
#else
    [SCP_TC3_SCMI_SERVICE_IDX_OSPM_A2P] = {
#endif
        .name = "OSPM_A2P",
        .data = &((
            struct mod_transport_channel_config){
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .policies =
                MOD_TRANSPORT_POLICY_INIT_MAILBOX,
            .out_band_mailbox_address = (uintptr_t)
                SCP_SCMI_PAYLOAD_NS_A2P_BASE,
            .out_band_mailbox_size =
                SCP_SCMI_PAYLOAD_SIZE,
            .driver_id = FWK_ID_SUB_ELEMENT_INIT(
                FWK_MODULE_IDX_MHU3,
                SCP_TC_MHU3_DEVICE_IDX_SCP_AP_NS,
                0),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_MHU3,
                0),
        }),
    },
#ifdef BUILD_HAS_MOD_TRANSPORT_FC
    [TC3_TRANSPORT_SCMI_PERF_FCH_CORTEX_A520_LEVEL_SET] = {
        .name = "FCH_CORTEX_A520_LEVEL_SET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_CORTEX_A520_LEVEL_SET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_CORTEX_A520_LIMIT_SET] = {
        .name = "FCH_CORTEX_A520_LIMIT_SET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_CORTEX_A520_LIMIT_SET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_CORTEX_A520_LEVEL_GET] = {
        .name = "FCH_CORTEX_A520_LEVEL_GET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_CORTEX_A520_LEVEL_GET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_CORTEX_A520_LIMIT_GET] = {
        .name = "FCH_CORTEX_A520_LIMIT_GET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_CORTEX_A520_LIMIT_GET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_CHABERTON_LEVEL_SET] = {
        .name = "FCH_CHABERTON_LEVEL_SET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_CHABERTON_LEVEL_SET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_CHABERTON_LIMIT_SET] = {
        .name = "FCH_CHABERTON_LIMIT_SET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_CHABERTON_LIMIT_SET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_CHABERTON_LEVEL_GET] = {
        .name = "FCH_CHABERTON_LEVEL_GET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_CHABERTON_LEVEL_GET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_CHABERTON_LIMIT_GET] = {
        .name = "FCH_CHABERTON_LIMIT_GET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_CHABERTON_LIMIT_GET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_BLACKHAWK_LEVEL_SET] = {
        .name = "FCH_BLACKHAWK_LEVEL_SET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_BLACKHAWK_LEVEL_SET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_BLACKHAWK_LIMIT_SET] = {
        .name = "FCH_BLACKHAWK_LIMIT_SET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_BLACKHAWK_LIMIT_SET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_BLACKHAWK_LEVEL_GET] = {
        .name = "FCH_BLACKHAWK_LEVEL_GET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_BLACKHAWK_LEVEL_GET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_BLACKHAWK_LIMIT_GET] = {
        .name = "FCH_BLACKHAWK_LIMIT_GET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_BLACKHAWK_LIMIT_GET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_GPU_LEVEL_SET] = {
        .name = "FCH_GPU_LEVEL_SET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_GPU_LEVEL_SET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_GPU_LIMIT_SET] = {
        .name = "FCH_GPU_LIMIT_SET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_GPU_LIMIT_SET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_GPU_LEVEL_GET] = {
        .name = "FCH_GPU_LEVEL_GET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_GPU_LEVEL_GET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
    [TC3_TRANSPORT_SCMI_PERF_FCH_GPU_LIMIT_GET] = {
        .name = "FCH_GPU_LIMIT_GET",
        .data = &((
            struct mod_transport_channel_config){
            .transport_type =
                MOD_TRANSPORT_CHANNEL_TRANSPORT_TYPE_FAST_CHANNELS,
            .channel_type =
                MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                TC3_PLAT_FCH_GPU_LIMIT_GET),
            .driver_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_FCH_POLLED,
                MOD_FCH_POLLED_API_IDX_TRANSPORT),
        }),
    },
#endif
#ifdef BUILD_HAS_MOD_TRANSPORT_FC
    [TC3_TRANSPORT_CHANNELS_COUNT] = { 0 },
#else
    [SCP_TC3_SCMI_SERVICE_IDX_COUNT] = { 0 },
#endif
};

static const struct fwk_element *transport_get_element_table(fwk_id_t module_id)
{
    unsigned int idx;
    struct mod_transport_channel_config *config;

#ifdef BUILD_HAS_MOD_TRANSPORT_FC
    for (idx = 0; idx < TC3_TRANSPORT_CHANNELS_COUNT; idx++) {
#else
    for (idx = 0; idx < SCP_TC3_SCMI_SERVICE_IDX_COUNT; idx++) {
#endif
        config =
            (struct mod_transport_channel_config *)(transport_element_table[idx]
                                                        .data);
        config->pd_source_id = FWK_ID_ELEMENT(
            FWK_MODULE_IDX_POWER_DOMAIN,
            tc3_core_get_core_count() + tc3_core_get_cluster_count() +
                PD_STATIC_DEV_IDX_SYSTOP);
    }

    return transport_element_table;
}

const struct fwk_module_config config_transport = {
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(transport_get_element_table),
};
