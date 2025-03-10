/*
 * Renesas SCP/MCP Software
 * Copyright (c) 2020-2024, Renesas Electronics Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <rcar_core.h>

#include <fwk_mmio.h>

#define PRR (0xFFF00044U)
#define PRR_57EN_OFS (27)
#define PRR_57EN_MASK (0x0f)
#define PRR_53EN_OFS (22)
#define PRR_53EN_MASK (0x0f)
#define PRR_CAEN_MASK \
    ((PRR_57EN_MASK << PRR_57EN_OFS) + (PRR_53EN_MASK << PRR_53EN_OFS))
#define RCAR_CLUSTER_COUNT  (2)

unsigned int rcar_core_get_count(void)
{
    return (__builtin_popcount(
        (fwk_mmio_read_32(PRR) & PRR_CAEN_MASK) ^ PRR_CAEN_MASK));
}

unsigned int rcar_cluster_get_count(void)
{
    return RCAR_CLUSTER_COUNT;
}
