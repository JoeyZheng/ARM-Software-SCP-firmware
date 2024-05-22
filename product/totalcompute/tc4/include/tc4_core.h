/*
 * Arm SCP/MCP Software
 * Copyright (c) 2023-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TC4_CORE_H
#define TC4_CORE_H

#include <fwk_assert.h>

#define TC4_GROUP_LITTLE_NAME   "GROUP_LITTLE"
#define TC4_GROUP_MID_NAME      "GROUP_MID"
#define TC4_GROUP_BIG_NAME      "GROUP_BIG"

#define TC4_CORES_PER_CLUSTER  (8)
#define TC4_NUMBER_OF_CLUSTERS (1)
#define TC4_NUMBER_OF_CORES (TC4_CORES_PER_CLUSTER * TC4_NUMBER_OF_CLUSTERS)

#define TC4_FOR_EACH_CORE(_func) \
    _func(0), \
    _func(1), \
    _func(2), \
    _func(3), \
    _func(4), \
    _func(5), \
    _func(6), \
    _func(7)

#define TC4_FOR_EACH_CLUSTER(_func) \
    _func(0)

#endif /* TC4_CORE_H */
