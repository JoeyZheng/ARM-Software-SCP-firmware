/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Definitions for PSU module configuration.
 */

#ifndef TC3_PSU_H
#define TC3_PSU_H

enum psu_id {
    PSU_ELEMENT_IDX_CORTEX_A520,
    PSU_ELEMENT_IDX_CHABERTON,
    PSU_ELEMENT_IDX_BLACKHAWK,
    PSU_ELEMENT_IDX_GPU,
    PSU_ELEMENT_IDX_COUNT,
};

#endif /* TC3_PSU_H */
