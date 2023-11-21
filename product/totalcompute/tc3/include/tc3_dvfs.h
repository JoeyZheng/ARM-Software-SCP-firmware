/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Definitions for DVFS module configuration.
 */

#ifndef TC3_DVFS_H
#define TC3_DVFS_H

enum dvfs_element_idx {
    DVFS_ELEMENT_IDX_CORTEX_A520,
    DVFS_ELEMENT_IDX_CHABERTON,
    DVFS_ELEMENT_IDX_BLACKHAWK,
    DVFS_ELEMENT_IDX_GPU,
    DVFS_ELEMENT_IDX_COUNT
};

#endif /* TC3_DVFS_H */
