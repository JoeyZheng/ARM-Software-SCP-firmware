/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Common definitions and utility functions for the CMN-Cyprus module.
 */

#include <internal/cmn_cyprus_common.h>

#include <fwk_assert.h>
#include <fwk_math.h>

#include <stdint.h>

uint64_t sam_encode_region_size(uint64_t region_size, uint64_t min_size)
{
    uint64_t blocks;

    /* Region size must be a multiple of minimum region size */
    fwk_assert((region_size % min_size) == 0);

    /* Region size also must be a power of two */
    fwk_assert((region_size & (region_size - 1)) == 0);

    blocks = region_size / min_size;
    return fwk_math_log2(blocks);
}

uint64_t sam_decode_region_size(uint64_t encoded_size, uint64_t min_size)
{
    uint64_t blocks;
    uint64_t size;

    /*
     * Calculate the number of blocks.
     * Encoded region size is binary logarithm of number of blocks.
     */
    blocks = fwk_math_pow2(encoded_size);

    /*
     * Calculate the size of the region.
     * Number of blocks = (size / min_size)
     */
    size = blocks * min_size;

    return size;
}

inline bool is_exp_raid_valid(uint16_t raid)
{
    return (raid <= MAX_EXP_RAID);
}

inline bool is_ccg_agent_id_valid(unsigned int agent_id)
{
    return (agent_id <= MAX_LOGICAL_AGENT_ID);
}

inline bool is_ccg_link_id_valid(uint8_t link_id)
{
    return (link_id == CCG_LINK_0);
}
