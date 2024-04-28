/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Configuration data for module 'mhu3'.
 */

#include "scp_cfgd_mhu3.h"
#include "scp_mmap.h"

#include <mod_mhu3.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>

#include <fmw_cmsis.h>

#define MOD_MHU3_ELEMENT_COUNT (SCP_CFGD_MOD_MHU3_EIDX_COUNT + 1)

/*
 * Timeout to wait for the receiver to clear the MHUv3 channel status
 * register so the channel can become available again.
 */
#define RESP_WAIT_TIMEOUT_US (30 * 1000)

/* AP<-->SCP Secure MHUv3 doorbell channel configuration */
struct mod_mhu3_channel_config scp2ap_s_dbch_config[1] = {
    /* PBX CH 0, FLAG 0, MBX CH 0, FLAG 0 */
    [0] = MOD_MHU3_INIT_DBCH(0, 0, 0, 0),
};

/* SCP<-->RSE Secure MHUv3 Doorbell channel configuration */
struct mod_mhu3_channel_config scp2rse_s_dbch_config[1] = {
    /* PBX CH 0, FLAG 0, MBX CH 0, FLAG 0 */
    [0] = MOD_MHU3_INIT_DBCH(0, 0, 0, 0),
};

/* AP<-->SCP Secure MHUv3 doorbell channel count */
#define SCP2AP_S_DBCH_COUNT FWK_ARRAY_SIZE(scp2ap_s_dbch_config)

/* SCP<-->RSE Secure MHUv3 Doorbell channel count */
#define SCP2RSE_S_DBCH_COUNT FWK_ARRAY_SIZE(scp2rse_s_dbch_config)

/* Module element table */
static const struct fwk_element mhu_element_table[MOD_MHU3_ELEMENT_COUNT]  = {
    [SCP_CFGD_MOD_MHU3_EIDX_SCP_AP_S] = {
        .name = "SCP2AP_S_MHU_DBCH",
        .sub_element_count = SCP2AP_S_DBCH_COUNT,
        .data = &(struct mod_mhu3_device_config) {
            .irq = (unsigned int) MHU3_AP2SCP_IRQ_S,
            .in = SCP_AP2SCP_MHUV3_RCV_S_BASE,
            .out = SCP_SCP2AP_MHUV3_SEND_S_BASE,
            .channels = &scp2ap_s_dbch_config[0],
            .timer_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_TIMER, 0),
            .resp_wait_timeout_us = RESP_WAIT_TIMEOUT_US,
        },
    },
    [SCP_CFGD_MOD_MHU3_EIDX_SCP_RSE_S] = {
        .name = "SCP2RSE_S_MHU_DBCH",
        .sub_element_count = SCP2RSE_S_DBCH_COUNT,
        .data = &(struct mod_mhu3_device_config) {
            .irq = (unsigned int) MHU3_RSE2SCP_IRQ_S,
            .in = SCP_RSE2SCP_MHUV3_RCV_S_BASE,
            .out = SCP_SCP2RSE_MHUV3_SEND_S_BASE,
            .channels = &scp2rse_s_dbch_config[0],
            .timer_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_TIMER, 0),
            .resp_wait_timeout_us = RESP_WAIT_TIMEOUT_US,
        },
    },
    [SCP_CFGD_MOD_MHU3_EIDX_COUNT] = { 0 },
};

static const struct fwk_element *get_element_table(fwk_id_t module_id)
{
    return mhu_element_table;
}

struct fwk_module_config config_mhu3 = {
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(get_element_table),
};
