/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <fwk_dt_config_dyn.h>
#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>


#ifdef FWK_MODULE_GEN_DYNAMIC_CLOCK
const struct fwk_element *dyn_get_element_table_clock(fwk_id_t module_id)
{
    const struct fwk_element *clock_dev_desc_table;

    clock_dev_desc_table = _static_get_element_table_clock();

    /*
     * When running under an FVP the I2S clock does not fully function,
     * as it requires the I2C hardware interrupt to properly work. This
     * is replaced with the mock clock.
     *
     * Clock table differences for platform variants now handed by DT 
     * include variants so no additional code needed
     */

    return clock_dev_desc_table;
}
#endif
