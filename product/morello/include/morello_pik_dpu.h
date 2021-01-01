/*
 * Arm SCP/MCP Software
 * Copyright (c) 2020-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MORELLO_PIK_DPU_H
#define MORELLO_PIK_DPU_H

#include <fwk_macros.h>

#include <stdint.h>

/*!
 * \brief DPU PIK register definitions
 */
struct pik_dpu_reg {
    FWK_R uint8_t RESERVED0[0x830];
    FWK_RW uint32_t ACLKDP_CTRL;
    FWK_RW uint32_t ACLKDP_DIV1;
    FWK_RW uint32_t ACLKDP_DIV2;
    uint8_t RESERVED3[0xA00 - 0x83C];
    FWK_R uint32_t CLKFORCE_STATUS;
    FWK_W uint32_t CLKFORCE_SET;
    FWK_W uint32_t CLKFORCE_CLR;
    uint8_t RESERVED4[0xFC0 - 0xA0C];
    FWK_RW uint32_t PCL_CONFIG;
    uint8_t RESERVED5[0xFD0 - 0xFC4];
    FWK_R uint32_t PID4;
    FWK_R uint32_t PID5;
    FWK_R uint32_t PID6;
    FWK_R uint32_t PID7;
    FWK_R uint32_t PID0;
    FWK_R uint32_t PID1;
    FWK_R uint32_t PID2;
    FWK_R uint32_t PID3;
    FWK_R uint32_t ID0;
    FWK_R uint32_t ID1;
    FWK_R uint32_t ID2;
    FWK_R uint32_t ID3;
};

#endif /* MORELLO_PIK_DPU_H */
