/*
 * Arm SCP/MCP Software
 * Copyright (c) 2018-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     PCIe controller configuration registers.
 */

#ifndef INTERNAL_PCIE_CTRL_APB_REG_H
#define INTERNAL_PCIE_CTRL_APB_REG_H

#include <fwk_macros.h>

#include <stdint.h>

/*!
 * \brief PCIe APB register definitions
 */
struct pcie_ctrl_apb_reg {
  FWK_W   uint32_t  RESET_CTRL;
  FWK_R   uint32_t  RESET_STATUS;
          uint8_t   RESERVED0[0x1000-0x8];
  FWK_RW  uint32_t  INT_CTRL;
          uint32_t  RESERVED1;
          uint32_t  RESERVED2;
          uint32_t  RESERVED3;
  FWK_RW  uint32_t  RP_CONFIG_IN;
  FWK_R   uint32_t  RP_CONFIG_OUT;
          uint32_t  RESERVED4;
  FWK_RW  uint32_t  RP_ERROR_CTRL;
  FWK_R   uint32_t  RP_DEBUG;
  FWK_RW  uint32_t  RP_L1_EXIT_CTRL;
  FWK_R   uint32_t  RP_LTSSM_STATE;
  FWK_R   uint32_t  PIPE_STATUS;
  FWK_R   uint32_t  PM_STATUS;
          uint32_t  RESERVED5;
          uint32_t  RESERVED6;
          uint32_t  RESERVED7;
  FWK_R   uint32_t  PMA_COMMON_STATUS;
  FWK_R   uint32_t  PMA_LANE_STATUS;
          uint8_t   RESERVED8[0x3000-0x1048];
  FWK_RW  uint32_t  MODE_CTRL;
  FWK_RW  uint32_t  PWR_STATE_CHANGE_CTRL;
  FWK_R   uint32_t  VF_PWR_STATE;
  FWK_RW  uint32_t  HOT_RESET_INT_CTRL;
  FWK_RW  uint32_t  FLR_RESET_INT_CTRL;
  FWK_RW  uint32_t  EP_MISC_CTRL;
  FWK_R   uint32_t  EP_MISC_STATUS;
  FWK_R   uint32_t  PF_TPH_STATUS;
  FWK_R   uint32_t  VF_TPH_STATUS;
  FWK_RW  uint32_t  CCIX_CTRL;
          uint8_t   RESERVED9[0xFFD0-0x3028];
  FWK_R   uint32_t  PID4;
  FWK_R   uint32_t  PID0;
  FWK_R   uint32_t  PID1;
  FWK_R   uint32_t  PID2;
  FWK_R   uint32_t  PID3;
  FWK_R   uint32_t  CID0;
  FWK_R   uint32_t  CID1;
  FWK_R   uint32_t  CID2;
  FWK_R   uint32_t  CID3;
};

#define RESET_CTRL_PHY_REL_POS         UINT32_C(0)
#define RESET_CTRL_RC_REL_POS          UINT32_C(1)
#define RESET_CTRL_HOT_RESET_POS       UINT32_C(3)

#define RESET_CTRL_PHY_REL_MASK        (1 << RESET_CTRL_PHY_REL_POS)
#define RESET_CTRL_RC_REL_MASK         (1 << RESET_CTRL_RC_REL_POS)
#define RESET_CTRL_HOT_RESET_MASK      (1 << RESET_CTRL_HOT_RESET_POS)

#define RESET_STATUS_PLL_ST_POS        UINT32_C(0)
#define RESET_STATUS_PHY_REL_ST_POS    UINT32_C(1)
#define RESET_STATUS_RC_ST_POS         UINT32_C(2)
#define RESET_STATUS_RC_REL_ST_POS     UINT32_C(4)
#define RESET_STATUS_HOT_RESET_ST_POS  UINT32_C(5)
#define RESET_STATUS_PM_ST_POS         UINT32_C(6)

#define RESET_STATUS_PLL_ST_MASK       (1 << RESET_STATUS_PLL_ST_POS)
#define RESET_STATUS_PHY_REL_ST_MASK   (1 << RESET_STATUS_PHY_REL_ST_POS)
#define RESET_STATUS_RC_ST_MASK        (1 << RESET_STATUS_RC_ST_POS)
#define RESET_STATUS_RC_REL_ST_MASK    (1 << RESET_STATUS_RC_REL_ST_POS)
#define RESET_STATUS_HOT_RESET_ST_MASK (1 << RESET_STATUS_HOT_RESET_ST_POS)
#define RESET_STATUS_PM_ST_MASK        (1 << RESET_STATUS_PM_ST_POS)

#define INT_CTRL_NEGOTIATED_SPD_IRQ_EN_POS          UINT32_C(0)
#define INT_CTRL_LINK_TRNG_DONE_IRQ_EN_POS          UINT32_C(1)
#define INT_CTRL_PLL_STATUS_IRQ_EN_POS              UINT32_C(2)

#define INT_CTRL_NEGOTIATED_SPD_IRQ_EN_MASK \
        (1 << INT_CTRL_NEGOTIATED_SPD_IRQ_EN_POS)
#define INT_CTRL_LINK_TRNG_DONE_IRQ_EN_MASK \
        (1 << INT_CTRL_LINK_TRNG_DONE_IRQ_EN_POS)
#define INT_CTRL_PLL_STATUS_IRQ_EN_MASK \
        (1 << INT_CTRL_PLL_STATUS_IRQ_EN_POS)

#define RP_CONFIG_IN_CLIENT_REQ_EXIT_L2_POS         UINT32_C(0)
#define RP_CONFIG_IN_LINK_TRNG_EN_POS               UINT32_C(1)
#define RP_CONFIG_IN_ARI_EN_POS                     UINT32_C(2)
#define RP_CONFIG_IN_LANE_CNT_IN_POS                UINT32_C(3)
#define RP_CONFIG_IN_PCIE_GEN_SEL_POS               UINT32_C(6)
#define RP_CONFIG_IN_SR_IOV_EN_POS                  UINT32_C(8)
#define RP_CONFIG_IN_GEN3_DC_BAL_DIS_POS            UINT32_C(9)
#define RP_CONFIG_IN_SRIS_EN_POS                    UINT32_C(10)
#define RP_CONFIG_IN_PMA_CMN_EXT_REFCLK_DET_POS     UINT32_C(11)
#define RP_CONFIG_IN_PMA_CMN_EXT_REFCLK_TERMEN_POS  UINT32_C(12)
#define RP_CONFIG_IN_NON_POSTED_REJ_POS             UINT32_C(13)

#define RP_CONFIG_IN_CLIENT_REQ_EXIT_L2_MASK \
        (1 << RP_CONFIG_IN_CLIENT_REQ_EXIT_L2_POS)
#define RP_CONFIG_IN_LINK_TRNG_EN_MASK \
        (1 << RP_CONFIG_IN_LINK_TRNG_EN_POS)
#define RP_CONFIG_IN_ARI_EN_MASK \
        (1 << RP_CONFIG_IN_ARI_EN_POS)
#define RP_CONFIG_IN_LANE_CNT_IN_MASK \
        (0x7 << RP_CONFIG_IN_LANE_CNT_IN_POS)
#define RP_CONFIG_IN_PCIE_GEN_SEL_MASK \
        (0x3 << RP_CONFIG_IN_PCIE_GEN_SEL_POS)
#define RP_CONFIG_IN_SR_IOV_EN_MASK \
        (1 << RP_CONFIG_IN_SR_IOV_EN_POS)
#define RP_CONFIG_IN_GEN3_DC_BAL_DIS_MASK \
        (1 << RP_CONFIG_IN_GEN3_DC_BAL_DIS_POS)
#define RP_CONFIG_IN_SRIS_EN_MASK \
        (1 << RP_CONFIG_IN_SRIS_EN_POS)
#define RP_CONFIG_IN_PMA_CMN_EXT_REFCLK_DET_MASK \
        (1 << RP_CONFIG_IN_PMA_CMN_EXT_REFCLK_DET_POS)
#define RP_CONFIG_IN_PMA_CMN_EXT_REFCLK_TERMEN_MASK \
        (1 << RP_CONFIG_IN_PMA_CMN_EXT_REFCLK_TERMEN_POS)
#define RP_CONFIG_IN_NON_POSTED_REJ_MASK \
        (1 << RP_CONFIG_IN_NON_POSTED_REJ_POS)

#define RP_CONFIG_OUT_OBFF_EN_POS                  UINT32_C(0)
#define RP_CONFIG_OUT_RCB_STATUS_POS               UINT32_C(2)
#define RP_CONFIG_OUT_MAX_PAYLOAD_SIZE_POS         UINT32_C(3)
#define RP_CONFIG_OUT_MAX_READREQ_SIZE_POS         UINT32_C(6)
#define RP_CONFIG_OUT_LINK_PWR_STATE_POS           UINT32_C(9)
#define RP_CONFIG_OUT_FN_PWR_STATE_POS             UINT32_C(16)
#define RP_CONFIG_OUT_NEGOTIATED_SPD_POS           UINT32_C(20)
#define RP_CONFIG_OUT_NEGOTIATED_LINK_WIDTH_POS    UINT32_C(22)
#define RP_CONFIG_OUT_LINK_STATUS_POS              UINT32_C(25)

#define RP_CONFIG_OUT_OBFF_EN_MASK \
        (0x3 << RP_CONFIG_OUT_OBFF_EN_POS)
#define RP_CONFIG_OUT_RCB_STATUS_MASK \
        (1 << RP_CONFIG_OUT_RCB_STATUS_POS)
#define RP_CONFIG_OUT_MAX_PAYLOAD_SIZE_MASK \
        (0x7 << RP_CONFIG_OUT_MAX_PAYLOAD_SIZE_POS)
#define RP_CONFIG_OUT_MAX_READREQ_SIZE_MASK \
        (0x7 << RP_CONFIG_OUT_MAX_READREQ_SIZE_POS)
#define RP_CONFIG_OUT_LINK_PWR_STATE_MASK \
        (0xF << RP_CONFIG_OUT_LINK_PWR_STATE_POS)
#define RP_CONFIG_OUT_FN_PWR_STATE_MASK \
        (0x7 << RP_CONFIG_OUT_FN_PWR_STATE_POS)
#define RP_CONFIG_OUT_NEGOTIATED_SPD_MASK \
        (0x3 << RP_CONFIG_OUT_NEGOTIATED_SPD_POS)
#define RP_CONFIG_OUT_NEGOTIATED_LINK_WIDTH_MASK \
        (0x7 << RP_CONFIG_OUT_NEGOTIATED_LINK_WIDTH_POS)
#define RP_CONFIG_OUT_LINK_STATUS_MASK \
        (0x3 << RP_CONFIG_OUT_LINK_STATUS_POS)

#define RP_ERROR_CTRL_UNCORRECTABLE_ERROR_IN_POS   UINT32_C(0)
#define RP_ERROR_CTRL_CORRECTABLE_ERROR_IN_POS     UINT32_C(1)

#define RP_ERROR_CTRL_UNCORRECTABLE_ERROR_IN_MASK \
        (1 << RP_ERROR_CTRL_UNCORRECTABLE_ERROR_IN_POS)
#define RP_ERROR_CTRL_CORRECTABLE_ERROR_IN_MASK \
        (1 << RP_ERROR_CTRL_CORRECTABLE_ERROR_IN_POS)

#define RP_LTSSM_STATE_POS             UINT32_C(0)
#define RP_LTSSM_STATE_MASK            (0x3F << RP_LTSSM_STATE_POS)

#define PIPE_STATUS_PIPE_RATE_POS      UINT32_C(0)
#define PIPE_STATUS_PIPE_RATE_MASK     (0x3 << PIPE_STATUS_PIPE_RATE_POS)

#define PM_STATUS_L1_PM_SUBSTATE_POS   UINT32_C(0)
#define PM_STATUS_L1_PM_SUBSTATE_MASK  (0x7 << PM_STATUS_L1_PM_SUBSTATE_POS)

#define MODE_CTRL_MODE_SELECT_EP       UINT32_C(0)
#define MODE_CTRL_MODE_SELECT_RP       UINT32_C(1)

#define EP_MISC_CTRL_REQ_PM_L23_READY_POS          UINT32_C(0)
#define EP_MISC_CTRL_CONFIG_EN_POS                 UINT32_C(8)

#define EP_MISC_CTRL_REQ_PM_L23_READY_MASK \
        (0x1 << EP_MISC_CTRL_REQ_PM_L23_READY_POS)
#define EP_MISC_CTRL_CONFIG_EN_MASK \
        (0x1 << EP_MISC_CTRL_CONFIG_EN_POS)

#endif  /* INTERNAL_PCIE_CTRL_APB_REG_H */
