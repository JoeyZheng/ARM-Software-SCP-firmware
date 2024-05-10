/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <mod_mhu.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>

#include <fwk_dt_config_common.h>

/* macros for extracting DT values and generating configuration table */
#define MHU_COMPAT  scp_mhu
#define MHU_NODE_ID(n) DT_INST(n, MHU_COMPAT)
#define MHU_PROP(n, prop)  FWK_DT_INST_PROP(MHU_COMPAT, n, prop)

/* table entry generator - array index is constant supplied by elem-idx in DT */
#define MHU_TABLE_ENTRY(n) \
    [MHU_PROP(n, elem_idx)] = {  \
        .name = "", \
        .sub_element_count = MHU_PROP(n, sub_elem_count), \
        .data = &(struct mod_mhu_device_config) { \
            .irq = (unsigned int)MHU_PROP(n, irq), \
            .in = DT_REG_ADDR_BY_IDX(MHU_NODE_ID(n), 1), \
            .out = DT_REG_ADDR_BY_IDX(MHU_NODE_ID(n), 0), \
        }, \
    }, 

static const struct fwk_element element_table[] = {
    /* macro for array elements */
    DT_FOREACH_OKAY_INST_scp_mhu(MHU_TABLE_ENTRY)
    /* last NULL element */
    [DT_N_INST_scp_mhu_NUM_OKAY] = { 0 },    
};

/* access data table from dynamic element callback */
#ifdef FWK_MODULE_GEN_DYNAMIC_MHU
const struct fwk_element *_static_get_element_table_mhu() {
    return element_table;
}
#endif

/* config structure */
struct fwk_module_config config_dt_mhu = {
#ifdef FWK_MODULE_GEN_DYNAMIC_MHU
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dyn_get_element_table_mhu),
#else
    .elements = FWK_MODULE_STATIC_ELEMENTS_PTR(element_table),
#endif
};
