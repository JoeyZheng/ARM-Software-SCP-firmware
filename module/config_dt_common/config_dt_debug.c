/*
 * Arm SCP/MCP Software
 * Copyright (c) 2020-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mod_debug.h>

#include <fwk_element.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <fwk_devicetree.h>
#include <fwk_dt_config_common.h>

/* SCP debug module configuration...

DT excerpt :

scp-debug@0 {
            compatible = "scp,debug";
            label = "JUNO DEBUG";
            scp-ids = <&elem_id FWK_MODULE_IDX_JUNO_DEBUG 0>,
                      <&api_id FWK_MODULE_IDX_JUNO_DEBUG MOD_JUNO_DEBUG_API_IDX_DRIVER>;
};

creates elements in array:

static const struct fwk_element debug_element_table[] = {
    [0] = {
        .name = "JUNO DEBUG",
        .data = &((const struct mod_debug_dev_config) {
            .driver_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_JUNO_DEBUG, 0),
            .driver_api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_JUNO_DEBUG,
                                             MOD_JUNO_DEBUG_API_IDX_DRIVER),
        }),
    },
    [1] = { 0 },  
};
*/

/* config table generation */

#define SCP_DEBUG_COMPAT scp_debug

#define  CFG_DEBUG_ELEM_DATA_PH(n) \
    .driver_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(SCP_DEBUG_COMPAT, n, 0), \
    .driver_api_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(SCP_DEBUG_COMPAT, n, 1),

#define CFG_DEBUG_ELEM_INIT(n) \
    [n] = { \
        .name = FWK_DT_INST_PROP(SCP_DEBUG_COMPAT, n, label), \
        .data = &((const struct mod_debug_dev_config)  { \
            CFG_DEBUG_ELEM_DATA_PH(n)  \
        }), \
    },

/* data table */
static struct fwk_element debug_element_table[] = {
        // Macro for array elements 
        DT_FOREACH_OKAY_INST_scp_debug(CFG_DEBUG_ELEM_INIT)
        // last null element
        [DT_N_INST_scp_debug_NUM_OKAY] = { 0 },
};

/* access data table from dynamic element callback */
#ifdef FWK_MODULE_GEN_DYNAMIC_SCP_DEBUG
const struct fwk_element *_static_get_element_table_debug() {
    return debug_element_table;
}
#endif

/* config structure */
struct fwk_module_config config_dt_debug = {
    #ifdef FWK_MODULE_GEN_DYNAMIC_SCP_DEBUG
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dyn_get_element_table_debug),
    #else
    .elements = FWK_MODULE_STATIC_ELEMENTS_PTR(debug_element_table),
    #endif
};