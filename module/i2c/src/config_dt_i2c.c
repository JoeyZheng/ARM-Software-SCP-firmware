/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023, Linaro Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mod_i2c.h"

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <fwk_dt_config_common.h>

/* compatible for the SCP module not the hardware */
#define MOD_I2C_COMPAT scp_mod_arm_apb_i2c

/*

 DT excerpt describing the scp module controlling i2c ...

...
        scp-mod-i2c {
            compatible = "scp,mod-arm-apb-i2c";
            label = "I2C";
            scp,hardware = <&i2c>;
            scp-ids = <&elem_id FWK_MODULE_IDX_DW_APB_I2C 0>,
                      <&api_id FWK_MODULE_IDX_DW_APB_I2C MOD_DW_APB_I2C_API_IDX_DRIVER>,
                      <&elem_id FWK_MODULE_IDX_TIMER 0>;
            scp-id-names = <"driver" "api" "timer">;
        };

...

static const struct fwk_element i2c_element_table[] = {
    [0] = {
        .name = "I2C",
        .data = &(struct mod_i2c_dev_config) {
            .driver_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_DW_APB_I2C, 0),
            .api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_DW_APB_I2C,
                MOD_DW_APB_I2C_API_IDX_DRIVER),
        },
    },
    [1] = {0},
};

*/

#define CFG_I2C_ELEMENT_DATA(n) \
    .driver_id = FWK_DT_PH_IDX_SCP_ID_BY_NAME(MOD_I2C_COMPAT, n, driver), \
    .api_id = FWK_DT_PH_IDX_SCP_ID_BY_NAME(MOD_I2C_COMPAT, n, api),

#define CFG_I2C_ELEMENT_INIT(n) \
    [n] = { \
        .name = FWK_DT_INST_PROP_DEFAULT(MOD_I2C_COMPAT, n, label, ""), \
        .data = &(struct mod_i2c_dev_config) {   \
            CFG_I2C_ELEMENT_DATA(n)              \
        },                                       \
   },

static const struct fwk_element i2c_element_table[] = {
    // Macro for array elements
    DT_FOREACH_OKAY_INST_scp_mod_arm_apb_i2c(CFG_I2C_ELEMENT_INIT)
    // last null element
    [DT_N_INST_scp_mod_arm_apb_i2c_NUM_OKAY] = { 0 },
};

/* access data table from dynamic element callback */
#ifdef FWK_MODULE_GEN_DYNAMIC_I2C
const struct fwk_element *_static_get_element_table_i2c() {
    return i2c_element_table;
}
#endif

/* config dt structure */
struct fwk_module_config config_dt_i2c = {
    #ifdef FWK_MODULE_GEN_DYNAMIC_I2C
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dyn_get_element_table_i2c),
    #else
    .elements = FWK_MODULE_STATIC_ELEMENTS_PTR(i2c_element_table),
    #endif
};