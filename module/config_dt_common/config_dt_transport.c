/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019-2022, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*#include "config_power_domain.h"
#include "juno_mhu.h"
#include "juno_scmi.h"
#include "software_mmap.h"
*/

#include <mod_transport.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

#include <fwk_devicetree.h>
#include <fwk_dt_config_common.h>

#include <stddef.h>
#include <stdint.h>

#if 0
static const struct fwk_element element_table[] = {
    /* JUNO_SCMI_SERVICE_IDX_PSCI_A2P */
    { .name = "",
      .data =
          &(struct mod_transport_channel_config){
              .channel_type = MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
              .policies =
                  (MOD_TRANSPORT_POLICY_INIT_MAILBOX |
                   MOD_TRANSPORT_POLICY_SECURE),
              .out_band_mailbox_address = (uintptr_t)SCMI_PAYLOAD_S_A2P_BASE,
              .out_band_mailbox_size = SCMI_PAYLOAD_SIZE,
              .driver_id = FWK_ID_SUB_ELEMENT_INIT(
                  FWK_MODULE_IDX_MHU,
                  JUNO_MHU_DEVICE_IDX_S,
                  0),
              .driver_api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_MHU, 0),
              .pd_source_id = FWK_ID_ELEMENT_INIT(
                  FWK_MODULE_IDX_POWER_DOMAIN,
                  POWER_DOMAIN_IDX_SYSTOP),
          } },
    /* JUNO_SCMI_SERVICE_IDX_OSPM_A2P_0 */
    { .name = "",
      .data =
          &(struct mod_transport_channel_config){
              .channel_type = MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
              .policies = MOD_TRANSPORT_POLICY_INIT_MAILBOX,
              .out_band_mailbox_address = (uintptr_t)SCMI_PAYLOAD_LOW_A2P_BASE,
              .out_band_mailbox_size = SCMI_PAYLOAD_SIZE,
              .driver_id = FWK_ID_SUB_ELEMENT_INIT(
                  FWK_MODULE_IDX_MHU,
                  JUNO_MHU_DEVICE_IDX_NS_L,
                  0),
              .driver_api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_MHU, 0),
              .pd_source_id = FWK_ID_ELEMENT_INIT(
                  FWK_MODULE_IDX_POWER_DOMAIN,
                  POWER_DOMAIN_IDX_SYSTOP),
          } },
    /* JUNO_SCMI_SERVICE_IDX_OSPM_A2P_1 */
    { .name = "",
      .data =
          &(struct mod_transport_channel_config){
              .channel_type = MOD_TRANSPORT_CHANNEL_TYPE_COMPLETER,
              .policies = MOD_TRANSPORT_POLICY_INIT_MAILBOX,
              .out_band_mailbox_address = (uintptr_t)SCMI_PAYLOAD_HIGH_A2P_BASE,
              .out_band_mailbox_size = SCMI_PAYLOAD_SIZE,
              .driver_id = FWK_ID_SUB_ELEMENT_INIT(
                  FWK_MODULE_IDX_MHU,
                  JUNO_MHU_DEVICE_IDX_NS_H,
                  0),
              .driver_api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_MHU, 0),
              .pd_source_id = FWK_ID_ELEMENT_INIT(
                  FWK_MODULE_IDX_POWER_DOMAIN,
                  POWER_DOMAIN_IDX_SYSTOP),
          } },
#ifdef BUILD_HAS_SCMI_NOTIFICATIONS
    /* JUNO_SCMI_SERVICE_IDX_OSPM_P2A */
    { .name = "",
      .data =
          &(struct mod_transport_channel_config){
              .channel_type = MOD_TRANSPORT_CHANNEL_TYPE_REQUESTER,
              .policies = MOD_TRANSPORT_POLICY_INIT_MAILBOX,
              .out_band_mailbox_address = (uintptr_t)SCMI_PAYLOAD_HIGH_P2A_BASE,
              .out_band_mailbox_size = SCMI_PAYLOAD_SIZE,
              .driver_id = FWK_ID_SUB_ELEMENT_INIT(
                  FWK_MODULE_IDX_MHU,
                  JUNO_MHU_DEVICE_IDX_NS_H,
                  1),
              .driver_api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_MHU, 0),
              .pd_source_id = FWK_ID_ELEMENT_INIT(
                  FWK_MODULE_IDX_POWER_DOMAIN,
                  POWER_DOMAIN_IDX_SYSTOP),
          } },
#endif
    [JUNO_SCMI_SERVICE_IDX_COUNT] = { 0 },
};
#endif


/* compatibles and extract DT values */
#define TRPT_COMPAT scp_transport
#define TRPT_PROP(n, prop) FWK_DT_INST_PROP(TRPT_COMPAT, n, prop)
#define TRPT_PROP_OR_DEF(n, prop, def) FWK_DT_INST_PROP_DEFAULT(TRPT_COMPAT, n, prop, def)
#define TRPT_NODE_ID(n) DT_INST(n, TRPT_COMPAT)
#define TRPT_SCP_ID_NAME(n, name) FWK_DT_PH_IDX_SCP_ID_BY_NAME(TRPT_COMPAT, n, name)
#define TRPT_SCP_ID_NAME_OR_NONE(n, name) FWK_DT_PH_IDX_SCP_ID_BY_NAME_OR_NONE(TRPT_COMPAT, n, name)

/* 
 * reg addr or size by index - 
 * only one value so idx is always 0 - 
 * just check it exists otherwise return 0 value 
 */
#define TRPT_REG_ADDR_OR_NONE(n) \
    COND_CODE_1(DT_REG_HAS_IDX(TRPT_NODE_ID(n), 0), \
    (DT_REG_ADDR_BY_IDX(TRPT_NODE_ID(n), 0)), (0))

#define TRPT_REG_SIZE_OR_NONE(n) \
    COND_CODE_1(DT_REG_HAS_IDX(TRPT_NODE_ID(n), 0), \
    (DT_REG_SIZE_BY_IDX(TRPT_NODE_ID(n), 0)), (0))

/* ID names */
#define TRPT_ID_DRIVER  driver
#define TRPT_ID_API     driver_api
#define TRPT_ID_PD_SRC  pd_source
#define TRPT_ID_SIG     signal_api

/* table generation */
#define TRPT_TABLE_ENTRY(n) [n] = { \
        .name = TRPT_PROP_OR_DEF(n, label, ""), \
        .data = &(struct mod_transport_channel_config) { \
        .transport_type = TRPT_PROP_OR_DEF(n, transport_type, 0), \
        .channel_type = TRPT_PROP(n, channel_type), \
        .policies = TRPT_PROP(n, channel_policies), \
        .in_band_mailbox_size = TRPT_PROP_OR_DEF(n, in_band_size, 0), \
        .out_band_mailbox_address = (uintptr_t)TRPT_REG_ADDR_OR_NONE(n), \
        .out_band_mailbox_size = TRPT_REG_SIZE_OR_NONE(n), \
        .driver_id = TRPT_SCP_ID_NAME(n, TRPT_ID_DRIVER), \
        .driver_api_id = TRPT_SCP_ID_NAME(n, TRPT_ID_API), \
        .pd_source_id = TRPT_SCP_ID_NAME_OR_NONE(n, TRPT_ID_PD_SRC), \
        .signal_api_id = TRPT_SCP_ID_NAME_OR_NONE(n, TRPT_ID_SIG), \
        }, \
    }, 

static const struct fwk_element element_table[] = {
    DT_FOREACH_OKAY_INST_scp_transport(TRPT_TABLE_ENTRY) 
    /* null termination */
    [DT_N_INST_scp_transport_NUM_OKAY] = { 0 },
};

static const struct fwk_element *get_element_table(fwk_id_t module_id)
{
    return element_table;
}

struct fwk_module_config config_dt_transport = {
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(get_element_table),
};

#ifdef FWK_MODULE_GEN_DYNAMIC_TRANSPORT
#error ("module transport cannot use the DT dynamic configuration")
#endif
