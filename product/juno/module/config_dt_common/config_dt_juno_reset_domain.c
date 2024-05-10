/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fwk_element.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <juno_scc.h>
#include <mod_juno_reset_domain.h>

#include <fwk_dt_config_common.h>

#define RST_DOM_COMPAT scp_mod_juno_reset
#define RST_DOM_HW scp_hardware
#define RST_DOM_PROP(n, prop) FWK_DT_INST_PROP(RST_DOM_COMPAT, n, prop)
    
#define RST_TAB_ELEM(n) \
    [RST_DOM_PROP(n, elem_idx)] = { \
        .name = RST_DOM_PROP(n, label),  \
        .data =  &((struct mod_juno_reset_uart_config) { \
            .reset_reg = (volatile uint32_t *)(FWK_DT_PH_INST_REG(RST_DOM_COMPAT, n, RST_DOM_HW, 0)), \
        }), \
    },

static struct fwk_element juno_reset_element_table[] = {
    // build elements 
    DT_FOREACH_OKAY_INST_scp_mod_juno_reset(RST_TAB_ELEM)
    // terminating element
    [DT_N_INST_scp_mod_juno_reset_NUM_OKAY] = { 0 },
};
 

static const struct fwk_element *get_juno_reset_domain_elem_table(
                                     fwk_id_t module_id)
{
    struct mod_juno_reset_uart_config *cfg;

    /* fixup uart reset element
     * reset reg contains SCC base - adjust with offset,
     * and set mask in element table 
     */
    cfg = (struct mod_juno_reset_uart_config *)juno_reset_element_table[JUNO_RESET_DOMAIN_IDX_UART].data;
    cfg->reset_reg = &(((struct scc_reg *)cfg->reset_reg)->VSYS_MANUAL_RESET);
    cfg->reset_mask = (0x1 << 8);
    return juno_reset_element_table;
}

struct fwk_module_config config_dt_juno_reset_domain = {
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(get_juno_reset_domain_elem_table),
};

#ifdef FWK_MODULE_GEN_DYNAMIC_JUNO_RESET_DOMAIN
#error("Error - juno reset domain DT module must be on static generator list")
#endif