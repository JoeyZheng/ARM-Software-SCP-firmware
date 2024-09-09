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

#endif /* TC4_CORE_H */
