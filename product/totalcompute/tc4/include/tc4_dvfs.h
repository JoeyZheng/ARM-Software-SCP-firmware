/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Definitions for DVFS module configuration.
 */

#ifndef TC4_DVFS_H
#define TC4_DVFS_H

enum dvfs_element_idx {
    DVFS_ELEMENT_IDX_NEVIS,
    DVFS_ELEMENT_IDX_GELAS,
    DVFS_ELEMENT_IDX_TRAVIS,
    DVFS_ELEMENT_IDX_GPU,
    DVFS_ELEMENT_IDX_COUNT
};

#endif /* TC4_DVFS_H */
