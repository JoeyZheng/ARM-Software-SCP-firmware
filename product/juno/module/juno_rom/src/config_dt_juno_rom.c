/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mod_juno_rom.h>

#include <fwk_dt_config_common.h>
#include <fwk_module.h>

#define J_ROM_INST DT_INST(0, arm_juno_rom)

static_assert(DT_N_INST_arm_juno_rom_NUM_OKAY == 1,
              "Error - multiple juno rom elements defined in device tree - must only be one");

struct fwk_module_config config_dt_juno_rom = {
    .data = &(struct mod_juno_rom_config) {
        .ap_context_base = DT_PROP_BY_IDX(J_ROM_INST, ap_context, 0),
        .ap_context_size = DT_PROP_BY_IDX(J_ROM_INST, ap_context, 1),
        .ramfw_base = DT_PROP(J_ROM_INST, ram_fw_base),
    },
};

#ifdef FWK_MODULE_GEN_DYNAMIC_JUNO_ROM
    #error("Juno-rom module does not support DT dynamic configuration")    
#endif
