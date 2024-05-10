/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023, Linaro Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mod_dw_apb_i2c.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <fwk_dt_config_common.h>

/* compatible for the SCP module not the hardware */
#define MOD_I2C_COMPAT scp_mod_arm_apb_i2c
#define PH_HW_NAME scp_hardware
#define I2C_APB_PH_PROP(n, prop) FWK_DT_PH_INST_PROP(MOD_I2C_COMPAT, n, PH_HW_NAME, prop)

/*

 DT excerpt describing the apb hardware and the scp module referencing it ...
...
        i2c: i2c@E0100000 {
            compatible = "arm,apb-i2c";
            reg = <0xE0100000 0x1000>;
            irq = <I2C_IRQ>;
        };

...
        scp-mod-i2c {
            compatible = "scp,mod-arm-apb-i2c";
            scp,hardware = <&i2c>;
            scp-ids = <&elem_id FWK_MODULE_IDX_DW_APB_I2C 0>,
                      <&api_id FWK_MODULE_IDX_DW_APB_I2C MOD_DW_APB_I2C_API_IDX_DRIVER>,
                      <&elem_id FWK_MODULE_IDX_TIMER 0>;
            scp-id-names = <"driver" "api" "timer">;
        };

...

construcing for the dw_apb_i2c module:-

static const struct fwk_element dw_apb_i2c_element_table[] = {
    [0] = {
        .name = "",
        .data =
            &(struct mod_dw_apb_i2c_dev_config){
                .timer_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_TIMER, 0),
                .i2c_irq = (unsigned int)I2C_IRQ,
                .reg = (uintptr_t)I2C_BASE,
        },
    },
    [1] = { 0 },
};
*/

/* register address and irq pulled from phandle reffed hw device via module object */
#define CFG_APB_I2C_ELEMENT_DATA(n)                                   \
    .reg = (uintptr_t)DT_REG_ADDR_BY_IDX(DT_PHANDLE(DT_INST(n, MOD_I2C_COMPAT), PH_HW_NAME), 0),   \
    .i2c_irq = (unsigned int)I2C_APB_PH_PROP(n, irq), \
    .timer_id = FWK_DT_PH_IDX_SCP_ID_BY_NAME(MOD_I2C_COMPAT, n, timer),

#define CFG_APB_I2C_ELEMENT_INIT(n) \
    [n] = { \
        .name = "", \
        .data = &(struct mod_dw_apb_i2c_dev_config) {   \
            CFG_APB_I2C_ELEMENT_DATA(n)                 \
        },                                              \
   },

static const struct fwk_element dw_apb_i2c_element_table[] = {
    // Macro for array elements
    DT_FOREACH_OKAY_INST_scp_mod_arm_apb_i2c(CFG_APB_I2C_ELEMENT_INIT)
    // last null element
    [DT_N_INST_scp_mod_arm_apb_i2c_NUM_OKAY] = { 0 },
};

/* access data table from dynamic element callback */
#ifdef FWK_MODULE_GEN_DYNAMIC_DW_APB_I2C
const struct fwk_element *_static_get_element_table_dw_apb_i2c() {
    return dw_apb_i2c_element_table;
}
#endif

/* config dt structure */
struct fwk_module_config config_dt_dw_apb_i2c = {
    #ifdef FWK_MODULE_GEN_DYNAMIC_DW_APB_I2C
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dyn_get_element_table_dw_apb_i2c),
    #else
    .elements = FWK_MODULE_STATIC_ELEMENTS_PTR(dw_apb_i2c_element_table),
    #endif
};