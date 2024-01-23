/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fwk_element.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

#include <fwk_dt_config_common.h>

#include <mod_reset_domain.h>

#define RST_DOM_CFG_COMPAT scp_reset_domain_config

static const struct mod_reset_domain_config reset_domain_config = {
#ifdef BUILD_HAS_NOTIFICATION
    .notification_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(RST_DOM_CFG_COMPAT, 0, 0),
#endif
};

/* Configuration of the reset elements */
#define RST_DOM_ELEM_COMPAT scp_reset_domain_elem
#define RST_DOM_PROP(n, prop) FWK_DT_INST_PROP(RST_DOM_ELEM_COMPAT, n ,prop)

/* single element .data structure */
#define RST_DOM_ELEM_DATA(n) \
        .driver_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(RST_DOM_ELEM_COMPAT, n, 0), \
        .driver_api_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(RST_DOM_ELEM_COMPAT, n, 1), \
        .latency = RST_DOM_PROP(n, latency), \
        .modes = RST_DOM_PROP(n, modes), \
        .capabilities = FWK_DT_INST_PROP_DEFAULT(RST_DOM_ELEM_COMPAT, n, capabilities, 0),

/* single element table entry */
#define RST_DOM_ELEM_INIT(n) \
    [RST_DOM_PROP(n, elem_idx)] = {  \
        .name = RST_DOM_PROP(n, label), \
        .data = &(struct mod_reset_domain_dev_config) { \
            RST_DOM_ELEM_DATA(n) \
        }, \
    },

/* table generator */
static struct fwk_element reset_domain_element_table[] = {
    /* Macro for array elements */
    DT_FOREACH_OKAY_INST_scp_reset_domain_elem(RST_DOM_ELEM_INIT)
    /* last null element */
    [DT_N_INST_scp_reset_domain_elem_NUM_OKAY] = { 0 },
};

/* access data table from dynamic element callback */
#ifdef FWK_MODULE_GEN_DYNAMIC_RESET_DOMAIN
static const struct fwk_element *_static_get_element_table_reset_domain(
                                     fwk_id_t module_id)
{
    return reset_domain_element_table;
}
#endif

/* Configuration of the reset domain module */
struct fwk_module_config config_dt_reset_domain = {
    .data = &reset_domain_config,
#ifdef FWK_MODULE_GEN_DYNAMIC_RESET_DOMAIN    
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dyn_get_element_table_reset_domain),
#else
    .elements = FWK_MODULE_STATIC_ELEMENTS_PTR(reset_domain_element_table),
#endif    
};
