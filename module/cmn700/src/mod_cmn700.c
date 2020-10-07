/*
 * Arm SCP/MCP Software
 * Copyright (c) 2020, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <cmn700.h>

#include <internal/cmn700_ctx.h>

#include <mod_clock.h>
#include <mod_cmn700.h>
#include <mod_system_info.h>
#include <mod_timer.h>

#include <fwk_assert.h>
#include <fwk_event.h>
#include <fwk_id.h>
#include <fwk_log.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <fwk_notification.h>
#include <fwk_status.h>

#include <inttypes.h>

#define MOD_NAME "[CMN700] "

static struct cmn700_device_ctx *ctx;

/* Chip Information */
static struct mod_system_info_get_info_api *system_info_api;
static const struct mod_system_info *system_info;

/* Initialize default for multi-chip mode and chip-id */
static unsigned int chip_id;
static bool multi_chip_mode;

static inline size_t cmn700_hnf_cache_group_count(size_t hnf_count)
{
    return (hnf_count + CMN700_HNF_CACHE_GROUP_ENTRIES_PER_GROUP - 1) /
        CMN700_HNF_CACHE_GROUP_ENTRIES_PER_GROUP;
}

static void process_node_hnf(struct cmn700_hnf_reg *hnf)
{
    unsigned int bit_pos;
    unsigned int group;
    unsigned int logical_id;
    unsigned int node_id;
    unsigned int region_idx;
    unsigned int region_sub_count = 0;
    uint64_t base;
    static unsigned int cal_mode_factor = 1;
    const struct mod_cmn700_mem_region_map *region;
    const struct mod_cmn700_config *config = ctx->config;

    logical_id = get_node_logical_id(hnf);
    node_id = get_node_id(hnf);

    /*
     * If CAL mode is set, only even numbered hnf node should be added to the
     * sys_cache_grp_hn_nodeid registers.
     */
    if (config->hnf_cal_mode == true && (node_id % 2 == 1)) {
        /* Factor to manipulate the group and bit_pos */
        cal_mode_factor = 2;
    }

    fwk_assert(logical_id < config->snf_count);

    group = logical_id /
        (CMN700_HNF_CACHE_GROUP_ENTRIES_PER_GROUP * cal_mode_factor);
    bit_pos = (CMN700_HNF_CACHE_GROUP_ENTRY_BITS_WIDTH / cal_mode_factor) *
        ((logical_id %
          (CMN700_HNF_CACHE_GROUP_ENTRIES_PER_GROUP * cal_mode_factor)));

    /*
     * If CAL mode is set, add only even numbered hnd node to
     * sys_cache_grp_hn_nodeid registers
     */
    if (config->hnf_cal_mode == true) {
        if (node_id % 2 == 0) {
            ctx->hnf_cache_group[group] += ((uint64_t)get_node_id(hnf))
                << bit_pos;
            ctx->sn_nodeid_group[group] +=
                ((uint64_t)config->snf_table[logical_id]) << bit_pos;
        }
    } else {
        ctx->hnf_cache_group[group] += ((uint64_t)get_node_id(hnf)) << bit_pos;
        ctx->sn_nodeid_group[group] += ((uint64_t)config->snf_table[logical_id])
            << bit_pos;
    }

    /* Set target node */
    hnf->SAM_CONTROL = config->snf_table[logical_id];

    /*
     * Map sub-regions to this HN-F node
     */
    for (region_idx = 0; region_idx < config->mmap_count; region_idx++) {
        region = &config->mmap_table[region_idx];

        /* Skip non sub-regions */
        if (region->type != MOD_CMN700_REGION_TYPE_SYSCACHE_SUB)
            continue;

        /* Offset the base with chip address space base on chip-id */
        base =
            ((uint64_t)(ctx->config->chip_addr_space * chip_id)) + region->base;

        /* Configure sub-region entry */
        if (get_hnsam_range_comp_en_mode(hnf)) {
            hnf->SAM_MEMREGION[region_sub_count] = region->node_id |
                ((base / SAM_GRANULARITY)
                 << CMN700_HNF_SAM_MEMREGION_BASE_POS) |
                CMN700_HNF_SAM_MEMREGION_VALID;
            hnf->SAM_MEMREGION_END_ADDR[region_sub_count] =
                ((base + region->size - 1));
        } else {
            hnf->SAM_MEMREGION[region_sub_count] = region->node_id |
                (sam_encode_region_size(region->size)
                 << CMN700_HNF_SAM_MEMREGION_SIZE_POS) |
                ((base / SAM_GRANULARITY)
                 << CMN700_HNF_SAM_MEMREGION_BASE_POS) |
                CMN700_HNF_SAM_MEMREGION_VALID;
        }
        region_sub_count++;
    }

    /* Configure the system cache RAM PPU */
    hnf->PPU_PWPR = CMN700_PPU_PWPR_POLICY_ON | CMN700_PPU_PWPR_OPMODE_FAM |
        CMN700_PPU_PWPR_DYN_EN;
}

/*
 * Discover the topology of the interconnect and identify the number of:
 * - External RN-SAM nodes
 * - Internal RN-SAM nodes
 * - HN-F nodes (cache)
 */
static int cmn700_discovery(void)
{
    unsigned int cxg_ra_reg_count;
    unsigned int cxg_ha_reg_count;
    unsigned int cxla_reg_count;
    unsigned int node_count;
    unsigned int node_idx;
    unsigned int xp_count;
    unsigned int xp_idx;
    unsigned int xp_port;
    struct cmn700_mxp_reg *xp;
    struct node_header *node;
    const struct mod_cmn700_config *config = ctx->config;

    cxg_ra_reg_count = 0;
    cxg_ha_reg_count = 0;
    cxla_reg_count = 0;

    FWK_LOG_INFO(MOD_NAME "Starting discovery...");
    FWK_LOG_TRACE(MOD_NAME "Rootnode Base address: 0x%x", (uintptr_t)ctx->root);

    fwk_assert(get_node_type(ctx->root) == NODE_TYPE_CFG);

    /* Traverse cross points (XP) */
    xp_count = get_node_child_count(ctx->root);
    for (xp_idx = 0; xp_idx < xp_count; xp_idx++) {
        xp = get_child_node(config->base, ctx->root, xp_idx);
        fwk_assert(get_node_type(xp) == NODE_TYPE_XP);

        FWK_LOG_INFO(
            MOD_NAME "XP (%d, %d) ID:%d, LID:%d",
            get_node_pos_x(xp),
            get_node_pos_y(xp),
            get_node_id(xp),
            get_node_logical_id(xp));

        /* Traverse nodes */
        node_count = get_node_child_count(xp);
        for (node_idx = 0; node_idx < node_count; node_idx++) {
            node = get_child_node(config->base, xp, node_idx);

            if (is_child_external(xp, node_idx)) { /* External nodes */
                xp_port = get_port_number(get_child_node_id(xp, node_idx));

                /*
                 * If the device type is CXRH, CXHA, or CXRA, then the external
                 * child node is CXLA as every CXRH, CXHA, or CXRA node has a
                 * corresponding external CXLA node.
                 */
                if ((get_device_type(xp, xp_port) == DEVICE_TYPE_CXRH) ||
                    (get_device_type(xp, xp_port) == DEVICE_TYPE_CXHA) ||
                    (get_device_type(xp, xp_port) == DEVICE_TYPE_CXRA)) {
                    cxla_reg_count++;
                    FWK_LOG_INFO(
                        MOD_NAME "  Found CXLA at node ID: %d",
                        get_child_node_id(xp, node_idx));
                } else if (get_device_type(xp, xp_port) == DEVICE_TYPE_CCG) {
                    FWK_LOG_INFO(
                        MOD_NAME "  Found CCG at node ID: %d",
                        get_child_node_id(xp, node_idx));
                } else { /* External RN-SAM Node */
                    ctx->external_rnsam_count++;
                    FWK_LOG_INFO(
                        MOD_NAME "  Found external node ID: %d addr: %p",
                        get_child_node_id(xp, node_idx),
                        xp);
                }
            } else { /* Internal nodes */
                switch (get_node_type(node)) {
                case NODE_TYPE_HN_F:
                    if (ctx->hnf_count >= MAX_HNF_COUNT) {
                        FWK_LOG_INFO(
                            MOD_NAME "  hnf count %d >= max limit (%d)",
                            ctx->hnf_count,
                            MAX_HNF_COUNT);
                        return FWK_E_DATA;
                    }
                    ctx->hnf_count++;
                    break;

                case NODE_TYPE_RN_SAM:
                    ctx->internal_rnsam_count++;

                    /*
                     * RN-F nodes does not have node type identifier and hence
                     * the count cannot be determined using the node type id.
                     * Alternatively, check if the device type connected to the
                     * Crosspoint (XP) is one of the RNF types and determine the
                     * RN-F count (if CAL connected RN-F, double the count).
                     */
                    xp_port = get_port_number(get_node_id(node));

                    if (is_device_type_rnf(xp, xp_port)) {
                        if (is_cal_connected(xp, xp_port)) {
                            ctx->rnf_count += 2;
                            FWK_LOG_INFO(
                                MOD_NAME
                                "  RN-F (CAL connected) found at port: %d",
                                xp_port);
                        } else {
                            ctx->rnf_count++;
                            FWK_LOG_INFO(
                                MOD_NAME "  RN-F found at port: %d", xp_port);
                        }
                    }
                    break;

                case NODE_TYPE_RN_D:
                    if ((ctx->rnd_count) >= MAX_RND_COUNT) {
                        FWK_LOG_ERR(
                            MOD_NAME "  rnd count %d >= max limit (%d)\n",
                            ctx->rnd_count,
                            MAX_RND_COUNT);
                        return FWK_E_DATA;
                    }
                    ctx->rnd_count++;
                    break;

                case NODE_TYPE_RN_I:
                    if ((ctx->rni_count) >= MAX_RNI_COUNT) {
                        FWK_LOG_ERR(
                            MOD_NAME "  rni count %d >= max limit (%d)\n",
                            ctx->rni_count,
                            MAX_RNI_COUNT);
                        return FWK_E_DATA;
                    }
                    ctx->rni_count++;
                    break;

                case NODE_TYPE_CXRA:
                    cxg_ra_reg_count++;
                    break;

                case NODE_TYPE_CXHA:
                    cxg_ha_reg_count++;
                    break;

                /* CXLA should not be an internal node */
                case NODE_TYPE_CXLA:
                    FWK_LOG_ERR(MOD_NAME
                                "CXLA node should not be internal node, "
                                "discovery failed");
                    return FWK_E_DEVICE;
                    break;

                default:
                    /* Nothing to be done for other node types */
                    break;
                }

                FWK_LOG_INFO(
                    MOD_NAME "  %s ID:%d, LID:%d",
                    get_node_type_name(get_node_type(node)),
                    get_node_id(node),
                    get_node_logical_id(node));
            }
        }
    }

    /* When CAL is present, the number of HN-Fs must be even. */
    if ((ctx->hnf_count % 2 != 0) && (config->hnf_cal_mode == true)) {
        FWK_LOG_ERR(
            MOD_NAME "hnf count: %d should be even when CAL mode is set",
            ctx->hnf_count);
        return FWK_E_DATA;
    }

    if (ctx->rnf_count > MAX_RNF_COUNT) {
        FWK_LOG_ERR(
            MOD_NAME "rnf count %d > max limit (%d)\n",
            ctx->rnf_count,
            MAX_RNF_COUNT);
        return FWK_E_RANGE;
    }

    FWK_LOG_INFO(
        MOD_NAME "Total internal RN-SAM nodes: %d", ctx->internal_rnsam_count);
    FWK_LOG_INFO(
        MOD_NAME "Total external RN-SAM nodes: %d", ctx->external_rnsam_count);
    FWK_LOG_INFO(MOD_NAME "Total HN-F nodes: %d", ctx->hnf_count);
    FWK_LOG_INFO(MOD_NAME "Total RN-D nodes: %d\n", ctx->rnd_count);
    FWK_LOG_INFO(MOD_NAME "Total RN-F nodes: %d\n", ctx->rnf_count);
    FWK_LOG_INFO(MOD_NAME "Total RN-I nodes: %d\n", ctx->rni_count);
    FWK_LOG_INFO(
        MOD_NAME "Total CCIX Request Agent nodes: %d", cxg_ra_reg_count);
    FWK_LOG_INFO(MOD_NAME "Total CCIX Home Agent nodes: %d", cxg_ha_reg_count);
    FWK_LOG_INFO(MOD_NAME "Total CCIX Link Agent nodes: %d", cxla_reg_count);

    return FWK_SUCCESS;
}

static void cmn700_configure(void)
{
    unsigned int hnf_entry;
    unsigned int node_count;
    unsigned int node_id;
    unsigned int node_idx;
    unsigned int xp_count;
    unsigned int xp_idx;
    unsigned int irnsam_entry;
    unsigned int xrnsam_entry;
    unsigned int xp_port;
    void *node;
    struct cmn700_mxp_reg *xp;
    const struct mod_cmn700_config *config = ctx->config;

    fwk_assert(get_node_type(ctx->root) == NODE_TYPE_CFG);

    hnf_entry = 0;
    irnsam_entry = 0;
    xrnsam_entry = 0;

    /* Traverse cross points (XP) */
    xp_count = get_node_child_count(ctx->root);
    for (xp_idx = 0; xp_idx < xp_count; xp_idx++) {
        xp = get_child_node(config->base, ctx->root, xp_idx);
        fwk_assert(get_node_type(xp) == NODE_TYPE_XP);

        /* Traverse nodes */
        node_count = get_node_child_count(xp);
        for (node_idx = 0; node_idx < node_count; node_idx++) {
            node = get_child_node(config->base, xp, node_idx);
            if (is_child_external(xp, node_idx)) {
                node_id = get_child_node_id(xp, node_idx);
                xp_port = get_port_number(get_child_node_id(xp, node_idx));

                if (!(get_device_type(xp, xp_port) == DEVICE_TYPE_CXRH) &&
                    !(get_device_type(xp, xp_port) == DEVICE_TYPE_CXHA) &&
                    !(get_device_type(xp, xp_port) == DEVICE_TYPE_CXRA)) {
                    fwk_assert(xrnsam_entry < ctx->external_rnsam_count);

                    ctx->external_rnsam_table[xrnsam_entry].node_id = node_id;
                    ctx->external_rnsam_table[xrnsam_entry].node = node;

                    xrnsam_entry++;
                }
            } else {
                enum node_type node_type = get_node_type(node);
                node_id = get_node_id(node);

                if (node_type == NODE_TYPE_RN_SAM) {
                    fwk_assert(irnsam_entry < ctx->internal_rnsam_count);

                    ctx->internal_rnsam_table[irnsam_entry] = node;

                    irnsam_entry++;
                } else if (node_type == NODE_TYPE_HN_F) {
                    fwk_assert(hnf_entry < ctx->hnf_count);
                    ctx->hnf_node[hnf_entry++] = (uintptr_t)(void *)node;

                    process_node_hnf(node);
                }
            }
        }
    }
}

static const char *const mmap_type_name[] = {
    [MOD_CMN700_MEM_REGION_TYPE_IO] = "I/O",
    [MOD_CMN700_MEM_REGION_TYPE_SYSCACHE] = "System Cache",
    [MOD_CMN700_REGION_TYPE_SYSCACHE_SUB] = "Sub-System Cache",
};

static int cmn700_setup_sam(struct cmn700_rnsam_reg *rnsam)
{
    unsigned int bit_pos;
    unsigned int group;
    unsigned int group_count;
    unsigned int hnf_count;
    unsigned int region_idx;
    unsigned int region_io_count = 0;
    unsigned int region_sys_count = 0;
    unsigned int scg_regions_enabled[MAX_SCG_COUNT] = { 0, 0, 0, 0 };
    uint64_t base;
    const struct mod_cmn700_mem_region_map *region;
    const struct mod_cmn700_config *config = ctx->config;

    FWK_LOG_INFO(MOD_NAME "Configuring SAM for node %d", get_node_id(rnsam));

    for (region_idx = 0; region_idx < config->mmap_count; region_idx++) {
        region = &config->mmap_table[region_idx];

        /* Offset the base with chip address space base on chip-id */
        base =
            ((uint64_t)(ctx->config->chip_addr_space * chip_id) + region->base);

        switch (region->type) {
        case MOD_CMN700_MEM_REGION_TYPE_IO:
            /*
             * Configure memory region
             */
            FWK_LOG_INFO(
                MOD_NAME "  [0x%" PRIx64 " - 0x%" PRIx64 "] %s",
                base,
                base + region->size - 1,
                mmap_type_name[region->type]);

            configure_region(
                rnsam,
                region_io_count,
                base,
                region->size,
                SAM_NODE_TYPE_HN_I,
                SAM_TYPE_NON_HASH_MEM_REGION);

            /*
             * Configure target node
             */
            group = region_io_count /
                CMN700_RNSAM_NON_HASH_TGT_NODEID_ENTRIES_PER_GROUP;
            bit_pos = CMN700_RNSAM_NON_HASH_TGT_NODEID_ENTRY_BITS_WIDTH *
                (region_io_count %
                 CMN700_RNSAM_NON_HASH_TGT_NODEID_ENTRIES_PER_GROUP);

            rnsam->NON_HASH_TGT_NODEID[group] &=
                ~(CMN700_RNSAM_NON_HASH_TGT_NODEID_ENTRY_MASK << bit_pos);
            rnsam->NON_HASH_TGT_NODEID[group] |=
                (region->node_id & CMN700_RNSAM_NON_HASH_TGT_NODEID_ENTRY_MASK)
                << bit_pos;

            region_io_count++;
            break;

        case MOD_CMN700_MEM_REGION_TYPE_SYSCACHE:
            /*
             * Configure memory region
             */
            FWK_LOG_INFO(
                MOD_NAME "  [0x%" PRIx64 " - 0x%" PRIx64 "] %s",
                region->base,
                region->base + region->size - 1,
                mmap_type_name[region->type]);

            configure_region(
                rnsam,
                region_sys_count,
                base,
                region->size,
                SAM_NODE_TYPE_HN_F,
                SAM_TYPE_SYS_CACHE_GRP_REGION);

            /* Mark corresponding region as enabled */
            fwk_assert(region_sys_count < MAX_SCG_COUNT);
            scg_regions_enabled[region_sys_count] = 1;

            region_sys_count++;
            break;

        case MOD_CMN700_REGION_TYPE_SYSCACHE_SUB:
            FWK_LOG_INFO(
                MOD_NAME "  [0x%" PRIx64 " - 0x%" PRIx64 "] %s",
                region->base,
                region->base + region->size - 1,
                mmap_type_name[region->type]);

            /* System cache sub-regions are handled by HN-Fs */
            break;

        default:
            fwk_unexpected();
            return FWK_E_DATA;
        }
    }

    /*
     * If CAL mode is enabled, then only the even numbered HN-F nodes are
     * programmed to the SYS_CACHE registers. Hence reduce the HN-F count by
     * half if CAL mode is enabled.
     */
    if (config->hnf_cal_mode)
        hnf_count = ctx->hnf_count / 2;
    else
        hnf_count = ctx->hnf_count;

    group_count = cmn700_hnf_cache_group_count(ctx->hnf_count);
    for (group = 0; group < group_count; group++)
        rnsam->SYS_CACHE_GRP_HN_NODEID[group] = ctx->hnf_cache_group[group];

    /* Program the number of HNFs */
    rnsam->SYS_CACHE_GRP_HN_COUNT = hnf_count;

    /*
     * If CAL mode is enabled by the configuration program the SCG CAL Mode
     * enable register.
     */
    if (config->hnf_cal_mode)
        for (region_idx = 0; region_idx < MAX_SCG_COUNT; region_idx++)
            rnsam->SYS_CACHE_GRP_CAL_MODE |= scg_regions_enabled[region_idx] *
                (CMN700_RNSAM_SCG_HNF_CAL_MODE_EN
                 << (region_idx * CMN700_RNSAM_SCG_HNF_CAL_MODE_SHIFT));

    /* Program the SYS_CACHE_GRP_SN_NODEID register for PrefetchTgt */
    if (config->hnf_cal_mode)
        group_count = config->snf_count /
            (CMN700_RNSAM_SYS_CACHE_GRP_SN_NODEID_ENTRIES_PER_GROUP * 2);
    else
        group_count = config->snf_count /
            CMN700_RNSAM_SYS_CACHE_GRP_SN_NODEID_ENTRIES_PER_GROUP;

    for (group = 0; group < group_count; group++)
        rnsam->SYS_CACHE_GRP_SN_NODEID[group] = ctx->sn_nodeid_group[group];

    /* Enable RNSAM */
    rnsam->STATUS = ((uint64_t)config->hnd_node_id
                     << CMN700_RNSAM_STATUS_DEFAULT_NODEID_POS) |
        CMN700_RNSAM_STATUS_UNSTALL;
    __sync_synchronize();

    return FWK_SUCCESS;
}

static int cmn700_setup(void)
{
    unsigned int rnsam_idx;
    int status = FWK_SUCCESS;

    if (!ctx->initialized) {
        status = cmn700_discovery();
        if (status != FWK_SUCCESS)
            return FWK_SUCCESS;

        /*
         * Allocate resources based on the discovery
         */

        /* Pointers for the internal RN-SAM nodes */
        if (ctx->internal_rnsam_count != 0) {
            ctx->internal_rnsam_table = fwk_mm_calloc(
                ctx->internal_rnsam_count, sizeof(*ctx->internal_rnsam_table));
        }

        /* Tuples for the external RN-RAM nodes (including their node IDs) */
        if (ctx->external_rnsam_count != 0) {
            ctx->external_rnsam_table = fwk_mm_calloc(
                ctx->external_rnsam_count, sizeof(*ctx->external_rnsam_table));
        }

        /* Cache groups */
        if (ctx->hnf_count != 0) {
            /*
             * Allocate enough group descriptors to accommodate all expected
             * HN-F nodes in the system.
             */
            ctx->hnf_node =
                fwk_mm_calloc(ctx->hnf_count, sizeof(*ctx->hnf_node));
            if (ctx->hnf_node == NULL)
                return FWK_E_NOMEM;
            ctx->hnf_cache_group = fwk_mm_calloc(
                cmn700_hnf_cache_group_count(ctx->hnf_count),
                sizeof(*ctx->hnf_cache_group));
            ctx->sn_nodeid_group = fwk_mm_calloc(
                ctx->hnf_count /
                    CMN700_RNSAM_SYS_CACHE_GRP_SN_NODEID_ENTRIES_PER_GROUP,
                sizeof(*ctx->sn_nodeid_group));
        }
    }

    cmn700_configure();

    /* Setup internal RN-SAM nodes */
    for (rnsam_idx = 0; rnsam_idx < ctx->internal_rnsam_count; rnsam_idx++)
        cmn700_setup_sam(ctx->internal_rnsam_table[rnsam_idx]);

    FWK_LOG_INFO(MOD_NAME "Done");

    ctx->initialized = true;

    return FWK_SUCCESS;
}

/*
 * Framework handlers
 */

static int cmn700_init(
    fwk_id_t module_id,
    unsigned int device_count,
    const void *data)
{
    /* Atleast one device should be passed as element */
    if (device_count == 0)
        return FWK_E_DATA;

    /* Allocate space for the device context table */
    ctx = fwk_mm_calloc(device_count, sizeof(struct cmn700_device_ctx));

    return FWK_SUCCESS;
}

static int cmn700_device_init(
    fwk_id_t element_id,
    unsigned int element_count,
    const void *data)
{
    struct cmn700_device_ctx *device_ctx;

    fwk_assert(data != NULL);

    device_ctx = ctx + fwk_id_get_element_idx(element_id);
    device_ctx->config = data;

    if (device_ctx->config->base == 0)
        return FWK_E_DATA;

    if ((device_ctx->config->mesh_size_x == 0) ||
        (device_ctx->config->mesh_size_x > CMN700_MESH_X_MAX))
        return FWK_E_DATA;

    if ((device_ctx->config->mesh_size_y == 0) ||
        (device_ctx->config->mesh_size_y > CMN700_MESH_Y_MAX))
        return FWK_E_DATA;

    if (device_ctx->config->snf_count > CMN700_HNF_CACHE_GROUP_ENTRIES_MAX)
        return FWK_E_DATA;

    device_ctx->root = get_root_node(
        device_ctx->config->base,
        device_ctx->config->hnd_node_id,
        device_ctx->config->mesh_size_x,
        device_ctx->config->mesh_size_y);

    return FWK_SUCCESS;
}

static int cmn700_bind(fwk_id_t id, unsigned int round)
{
    int status;
    struct cmn700_device_ctx *device_ctx;

    if (fwk_id_is_type(id, FWK_ID_TYPE_MODULE)) {
        /* Bind to system info module to obtain multi-chip info */
        status = fwk_module_bind(
            FWK_ID_MODULE(FWK_MODULE_IDX_SYSTEM_INFO),
            FWK_ID_API(FWK_MODULE_IDX_SYSTEM_INFO, MOD_SYSTEM_INFO_GET_API_IDX),
            &system_info_api);
        return status;
    }

    /* Use second round only (round numbering is zero-indexed) */
    if (round == 1) {
        device_ctx = ctx + fwk_id_get_element_idx(id);

        /* Bind to the timer component */
        status = fwk_module_bind(
            FWK_ID_ELEMENT(FWK_MODULE_IDX_TIMER, 0),
            FWK_ID_API(FWK_MODULE_IDX_TIMER, MOD_TIMER_API_IDX_TIMER),
            &device_ctx->timer_api);
        if (status != FWK_SUCCESS)
            return FWK_E_PANIC;
    }

    return FWK_SUCCESS;
}

int cmn700_start(fwk_id_t id)
{
    int status;

    /* No need to do anything for element */
    if (!fwk_module_is_valid_element_id(id))
        return FWK_SUCCESS;

    status = system_info_api->get_system_info(&system_info);
    if (status == FWK_SUCCESS) {
        chip_id = system_info->chip_id;
        multi_chip_mode = system_info->multi_chip_mode;
    }

    /* No need to anything for other elements */
    if (fwk_id_get_element_idx(id) != chip_id)
        return FWK_SUCCESS;

    /* Pickup the context based on the chip_id */
    ctx = ctx + fwk_id_get_element_idx(id);

    FWK_LOG_INFO(
        MOD_NAME "Multichip mode: %s",
        multi_chip_mode ? "Enabled" : "Disabled");
    FWK_LOG_INFO(MOD_NAME "Chip ID: %d", chip_id);

    if (fwk_id_is_equal(ctx->config->clock_id, FWK_ID_NONE)) {
        cmn700_setup();
        return FWK_SUCCESS;
    }

    /* Register the module for clock state notifications */
    return fwk_notification_subscribe(
        mod_clock_notification_id_state_changed, ctx->config->clock_id, id);
}

static int cmn700_process_notification(
    const struct fwk_event *event,
    struct fwk_event *resp_event)
{
    struct clock_notification_params *params;

    fwk_assert(
        fwk_id_is_equal(event->id, mod_clock_notification_id_state_changed));
    fwk_assert(fwk_id_is_type(event->target_id, FWK_ID_TYPE_ELEMENT));

    params = (struct clock_notification_params *)event->params;
    if (params->new_state == MOD_CLOCK_STATE_RUNNING) {
        cmn700_setup();
    }

    return FWK_SUCCESS;
}

const struct fwk_module module_cmn700 = {
    .name = "CMN700",
    .type = FWK_MODULE_TYPE_DRIVER,
    .init = cmn700_init,
    .element_init = cmn700_device_init,
    .bind = cmn700_bind,
    .start = cmn700_start,
    .process_notification = cmn700_process_notification,
};
