/*
 * Arm SCP/MCP Software
 * Copyright (c) 2017-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fwk_element.h>
#include <fwk_macros.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <mod_system_power.h>
#include <mod_power_domain.h>
#include <mod_ppu_v1.h>
#include <clark_power_domain.h>
#include <clark_core.h>
#include <config_ppu_v0.h>
#include <config_power_domain.h>

/* Maximum power domain name size including the null terminator */
#define PD_NAME_SIZE 12

/* Mask of the allowed states for the systop power domain */
static const uint32_t systop_allowed_state_mask_table[] = {
    [0] = MOD_PD_STATE_OFF_MASK | MOD_PD_STATE_ON_MASK |
          (1 << MOD_SYSTEM_POWER_POWER_STATE_SLEEP0) |
          (1 << MOD_SYSTEM_POWER_POWER_STATE_SLEEP1)
};

/*
 * Mask of the allowed states for the top level power domains
 * (but the cluster power domains) depending on the system states.
 */
static const uint32_t toplevel_allowed_state_mask_table[] = {
    [MOD_PD_STATE_OFF] = MOD_PD_STATE_OFF_MASK,
    [MOD_PD_STATE_ON] = MOD_PD_STATE_OFF_MASK | MOD_PD_STATE_ON_MASK,
    [MOD_SYSTEM_POWER_POWER_STATE_SLEEP0] = MOD_PD_STATE_OFF_MASK,
    [MOD_SYSTEM_POWER_POWER_STATE_SLEEP1] = MOD_PD_STATE_OFF_MASK
};

/*
 * Mask of the allowed states for the cluster power domain depending on the
 * system states.
 */
static const uint32_t cluster_pd_allowed_state_mask_table[] = {
    [MOD_PD_STATE_OFF] = MOD_PD_STATE_OFF_MASK | MOD_PD_STATE_SLEEP_MASK,
    [MOD_PD_STATE_ON] = CLARK_CLUSTER_VALID_STATE_MASK,
    [MOD_SYSTEM_POWER_POWER_STATE_SLEEP0] = MOD_PD_STATE_OFF_MASK,
    [MOD_SYSTEM_POWER_POWER_STATE_SLEEP1] = MOD_PD_STATE_OFF_MASK
};

/* Mask of the allowed states for a core depending on the cluster states. */
static const uint32_t core_pd_allowed_state_mask_table[] = {
    [MOD_PD_STATE_OFF] = MOD_PD_STATE_OFF_MASK | MOD_PD_STATE_SLEEP_MASK,
    [MOD_PD_STATE_ON] = CLARK_CORE_VALID_STATE_MASK,
    [MOD_PD_STATE_SLEEP] = MOD_PD_STATE_OFF_MASK | MOD_PD_STATE_SLEEP_MASK,
    [CLARK_POWER_DOMAIN_STATE_FUNC_RET] = CLARK_CORE_VALID_STATE_MASK,
    [CLARK_POWER_DOMAIN_STATE_MEM_RET] = MOD_PD_STATE_OFF_MASK
};

/* Power module specific configuration data (none) */
static const struct mod_power_domain_config
    clark_power_domain_config = { 0 };

static struct fwk_element clark_power_domain_static_element_table[] = {
    [PD_STATIC_DEV_IDX_CLUSTER0] = {
        .name = "CLUS0",
        .data = &((struct mod_power_domain_element_config) {
            .attributes.pd_type = MOD_PD_TYPE_CLUSTER,
            .tree_pos = MOD_PD_TREE_POS(
                MOD_PD_LEVEL_1, 0, 0, PD_STATIC_DEV_IDX_CLUSTER0, 0),
            .api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_PPU_V1,
                MOD_PPU_V1_API_IDX_POWER_DOMAIN_DRIVER),
            .allowed_state_mask_table = cluster_pd_allowed_state_mask_table,
            .allowed_state_mask_table_size =
                FWK_ARRAY_SIZE(cluster_pd_allowed_state_mask_table)
        }),
    },
    [PD_STATIC_DEV_IDX_CLUSTER1] = {
        .name = "CLUS1",
        .data = &((struct mod_power_domain_element_config) {
            .attributes.pd_type = MOD_PD_TYPE_CLUSTER,
            .tree_pos = MOD_PD_TREE_POS(
                MOD_PD_LEVEL_1, 0, 0, PD_STATIC_DEV_IDX_CLUSTER1, 0),
            .api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_PPU_V1,
                MOD_PPU_V1_API_IDX_POWER_DOMAIN_DRIVER),
            .allowed_state_mask_table = cluster_pd_allowed_state_mask_table,
            .allowed_state_mask_table_size =
                FWK_ARRAY_SIZE(cluster_pd_allowed_state_mask_table)
        }),
    },
    [PD_STATIC_DEV_IDX_DBGTOP] = {
        .name = "DBGTOP",
        .data = &((struct mod_power_domain_element_config) {
            .attributes.pd_type = MOD_PD_TYPE_DEVICE_DEBUG,
            .tree_pos = MOD_PD_TREE_POS(
                MOD_PD_LEVEL_1, 0, 0, PD_STATIC_DEV_IDX_DBGTOP, 0),
            .driver_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_PPU_V0, PPU_V0_ELEMENT_IDX_DBGTOP),
            .api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_PPU_V0, 0),
            .allowed_state_mask_table = toplevel_allowed_state_mask_table,
            .allowed_state_mask_table_size =
                FWK_ARRAY_SIZE(toplevel_allowed_state_mask_table)
        }),
    },
    [PD_STATIC_DEV_IDX_SYSTOP] = {
        .name = "SYSTOP",
        .data = &((struct mod_power_domain_element_config) {
            .attributes.pd_type = MOD_PD_TYPE_SYSTEM,
            .tree_pos = MOD_PD_TREE_POS(
                MOD_PD_LEVEL_2, 0, 0, 0, 0),
            .driver_id = FWK_ID_MODULE_INIT(FWK_MODULE_IDX_SYSTEM_POWER),
            .api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_SYSTEM_POWER,
                MOD_SYSTEM_POWER_API_IDX_PD_DRIVER),
            .allowed_state_mask_table = systop_allowed_state_mask_table,
            .allowed_state_mask_table_size =
                FWK_ARRAY_SIZE(systop_allowed_state_mask_table)
        }),
    },
};

/*
 * Function definitions with internal linkage
 */
static const struct fwk_element *clark_power_domain_get_element_table
    (fwk_id_t module_id)
{
    struct fwk_element *element_table, *element;
    struct mod_power_domain_element_config *pd_config_table, *pd_config;
    unsigned int core_idx;
    unsigned int cluster_idx;
    unsigned int core_count;
    unsigned int cluster_count;
    unsigned int core_element_count = 0;

    core_count = clark_core_get_core_count();
    cluster_count = clark_core_get_cluster_count();

    element_table = fwk_mm_calloc(
        core_count
        + FWK_ARRAY_SIZE(clark_power_domain_static_element_table)
        + 1, /* Terminator */
        sizeof(struct fwk_element));
    if (element_table == NULL)
        return NULL;

    pd_config_table = fwk_mm_calloc(core_count,
        sizeof(struct mod_power_domain_element_config));
    if (pd_config_table == NULL)
        return NULL;

    for (cluster_idx = 0; cluster_idx < cluster_count; cluster_idx++) {
        for (core_idx = 0;
             core_idx < clark_core_get_core_per_cluster_count(cluster_idx);
             core_idx++) {

            element = &element_table[core_element_count];
            pd_config = &pd_config_table[core_element_count];

            element->name = fwk_mm_alloc(PD_NAME_SIZE, 1);
            if (element->name == NULL)
                return NULL;

            snprintf((char *)element->name, PD_NAME_SIZE, "CLUS%uCORE%u",
                cluster_idx, core_idx);

            element->data = pd_config;

            pd_config->attributes.pd_type = MOD_PD_TYPE_CORE;
            pd_config->tree_pos = MOD_PD_TREE_POS(
                MOD_PD_LEVEL_0, 0, 0, cluster_idx, core_idx);
            pd_config->driver_id =
                FWK_ID_ELEMENT(FWK_MODULE_IDX_PPU_V1,
                               core_element_count);
            pd_config->api_id = FWK_ID_API(
                FWK_MODULE_IDX_PPU_V1,
                MOD_PPU_V1_API_IDX_POWER_DOMAIN_DRIVER);
            pd_config->allowed_state_mask_table =
                core_pd_allowed_state_mask_table;
            pd_config->allowed_state_mask_table_size =
                FWK_ARRAY_SIZE(core_pd_allowed_state_mask_table);
            core_element_count++;
        }

        /* Define the driver id for the cluster */
        pd_config = (struct mod_power_domain_element_config *)
            clark_power_domain_static_element_table[cluster_idx].data;
        pd_config->driver_id =
            FWK_ID_ELEMENT(FWK_MODULE_IDX_PPU_V1,
                           (core_count + cluster_idx));
    }

    memcpy(element_table + core_count,
           clark_power_domain_static_element_table,
           sizeof(clark_power_domain_static_element_table));

    return element_table;
}

/*
 * Power module configuration data
 */
const struct fwk_module_config config_power_domain = {
    .get_element_table = clark_power_domain_get_element_table,
    .data = &clark_power_domain_config,
};
