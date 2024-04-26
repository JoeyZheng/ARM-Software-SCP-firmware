/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Configuration data for module 'gtimer'.
 */

#include "scp_mmap.h"

#include <mod_gtimer.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>
#include <fwk_time.h>

#define MOD_GTIMER_ELEMENT_COUNT 2

/* REF_CLK input clock speed */
#define CLOCK_RATE_REFCLK (100UL * FWK_MHZ)

/*
 * System Counter per-tick increment value required for 100MHz clock speed as
 * required for SBSA compliance. That is, (100MHz / CLOCK_RATE_REFCLK) = 1.
 */
#define SYSCNT_INCR 1

/* Generic timer driver config */
static const struct fwk_element gtimer_dev_table[MOD_GTIMER_ELEMENT_COUNT] = {
    [0] = { .name = "REFCLK",
            .data = &((struct mod_gtimer_dev_config){
                .hw_timer = SCP_REFCLK_CNTBASE0_BASE,
                .hw_counter = SCP_REFCLK_CNTCTL_BASE,
                .control = SCP_REFCLK_CNTCONTROL_BASE,
                .frequency = (CLOCK_RATE_REFCLK * SYSCNT_INCR),
                .clock_id = FWK_ID_NONE_INIT,
            }) },
    [1] = { 0 },
};

const struct fwk_module_config config_gtimer = {
    .elements = FWK_MODULE_STATIC_ELEMENTS_PTR(gtimer_dev_table),
};

struct fwk_time_driver fmw_time_driver(const void **ctx)
{
    return mod_gtimer_driver(ctx, config_gtimer.elements.table[0].data);
}
