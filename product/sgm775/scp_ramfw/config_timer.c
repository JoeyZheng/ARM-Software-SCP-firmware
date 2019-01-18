/*
 * Arm SCP/MCP Software
 * Copyright (c) 2017-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fwk_id.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <mod_clock.h>
#include <mod_gtimer.h>
#include <mod_timer.h>
#include <sgm775_mmap.h>
#include <sgm775_irq.h>
#include <clock_devices.h>
#include <system_clock.h>

/*
 * Generic timer driver config
 */
static const struct fwk_element gtimer_dev_table[] = {
    [0] = {
        .name = "REFCLK",
        .data = &((struct mod_gtimer_dev_config) {
            .hw_timer   = REFCLK_CNTBASE0_BASE,
            .hw_counter = REFCLK_CNTCTL_BASE,
            .control    = REFCLK_CNTCONTROL_BASE,
            .frequency = CLOCK_RATE_REFCLK,
            .clock_id = FWK_ID_ELEMENT_INIT(
                            FWK_MODULE_IDX_CLOCK,
                            CLOCK_DEV_IDX_FCMCLK),
        })
    },
    [1] = { 0 },
};

static const struct fwk_element *gtimer_get_dev_table(fwk_id_t module_id)
{
    return gtimer_dev_table;
}

struct fwk_module_config config_gtimer = {
    .get_element_table = gtimer_get_dev_table,
};

/*
 * Timer HAL config
 */
static const struct mod_timer_dev_config refclk_config = {
    .id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_GTIMER, 0),
    .timer_irq = TIMREFCLK_IRQ,
};

static const struct fwk_element timer_dev_table[] = {
    [0] = {
        .name = "REFCLK",
        .data = &refclk_config,
        .sub_element_count = 8, /* Number of alarms */
    },
    [1] = { 0 },
};

static const struct fwk_element *timer_get_dev_table(fwk_id_t module_id)
{
    return timer_dev_table;
}

struct fwk_module_config config_timer = {
    .get_element_table = timer_get_dev_table,
};
