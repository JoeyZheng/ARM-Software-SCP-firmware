/*
 * Renesas SCP/MCP Software
 * Copyright (c) 2020-2021, Renesas Electronics Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RESET_MSTP_DEVICES_H
#define RESET_MSTP_DEVICES_H

/*!
 * \brief Clock device indexes.
 */
enum reset_mstp_dev_idx {
    RESET_MSTP_DEV_IDX_FDP1_1,
    RESET_MSTP_DEV_IDX_FDP1_0,
    RESET_MSTP_DEV_IDX_SCIF5,
    RESET_MSTP_DEV_IDX_SCIF4,
    RESET_MSTP_DEV_IDX_SCIF3,
    RESET_MSTP_DEV_IDX_SCIF1,
    RESET_MSTP_DEV_IDX_SCIF0,
    RESET_MSTP_DEV_IDX_MSIOF3,
    RESET_MSTP_DEV_IDX_MSIOF2,
    RESET_MSTP_DEV_IDX_MSIOF1,
    RESET_MSTP_DEV_IDX_MSIOF0,
    RESET_MSTP_DEV_IDX_SYS_DMAC2,
    RESET_MSTP_DEV_IDX_SYS_DMAC1,
    RESET_MSTP_DEV_IDX_SYS_DMAC0,
    RESET_MSTP_DEV_IDX_SCEG_PUB,
    RESET_MSTP_DEV_IDX_CMT3,
    RESET_MSTP_DEV_IDX_CMT2,
    RESET_MSTP_DEV_IDX_CMT1,
    RESET_MSTP_DEV_IDX_CMT0,
    RESET_MSTP_DEV_IDX_TPU0,
    RESET_MSTP_DEV_IDX_SCIF2,
    RESET_MSTP_DEV_IDX_SDIF3,
    RESET_MSTP_DEV_IDX_SDIF2,
    RESET_MSTP_DEV_IDX_SDIF1,
    RESET_MSTP_DEV_IDX_SDIF0,
    RESET_MSTP_DEV_IDX_PCIE1,
    RESET_MSTP_DEV_IDX_PCIE0,
    RESET_MSTP_DEV_IDX_USB_DMAC30,
    RESET_MSTP_DEV_IDX_USB3_IF0,
    RESET_MSTP_DEV_IDX_USB_DMAC31,
    RESET_MSTP_DEV_IDX_USB_DMAC0,
    RESET_MSTP_DEV_IDX_USB_DMAC1,
    RESET_MSTP_DEV_IDX_RWDT,
    RESET_MSTP_DEV_IDX_INTC_EX,
    RESET_MSTP_DEV_IDX_INTC_AP,
    RESET_MSTP_DEV_IDX_AUDMAC1,
    RESET_MSTP_DEV_IDX_AUDMAC0,
    RESET_MSTP_DEV_IDX_DRIF31,
    RESET_MSTP_DEV_IDX_DRIF30,
    RESET_MSTP_DEV_IDX_DRIF21,
    RESET_MSTP_DEV_IDX_DRIF20,
    RESET_MSTP_DEV_IDX_DRIF11,
    RESET_MSTP_DEV_IDX_DRIF10,
    RESET_MSTP_DEV_IDX_DRIF01,
    RESET_MSTP_DEV_IDX_DRIF00,
    RESET_MSTP_DEV_IDX_HSCIF4,
    RESET_MSTP_DEV_IDX_HSCIF3,
    RESET_MSTP_DEV_IDX_HSCIF2,
    RESET_MSTP_DEV_IDX_HSCIF1,
    RESET_MSTP_DEV_IDX_HSCIF0,
    RESET_MSTP_DEV_IDX_THERMAL,
    RESET_MSTP_DEV_IDX_PWM,
    RESET_MSTP_DEV_IDX_FCPVD2,
    RESET_MSTP_DEV_IDX_FCPVD1,
    RESET_MSTP_DEV_IDX_FCPVD0,
    RESET_MSTP_DEV_IDX_FCPVB1,
    RESET_MSTP_DEV_IDX_FCPVB0,
    RESET_MSTP_DEV_IDX_FCPVI1,
    RESET_MSTP_DEV_IDX_FCPVI0,
    RESET_MSTP_DEV_IDX_FCPF1,
    RESET_MSTP_DEV_IDX_FCPF0,
    RESET_MSTP_DEV_IDX_FCPCS,
    RESET_MSTP_DEV_IDX_VSPD2,
    RESET_MSTP_DEV_IDX_VSPD1,
    RESET_MSTP_DEV_IDX_VSPD0,
    RESET_MSTP_DEV_IDX_VSPBC,
    RESET_MSTP_DEV_IDX_VSPBD,
    RESET_MSTP_DEV_IDX_VSPI1,
    RESET_MSTP_DEV_IDX_VSPI0,
    RESET_MSTP_DEV_IDX_EHCI3,
    RESET_MSTP_DEV_IDX_EHCI2,
    RESET_MSTP_DEV_IDX_EHCI1,
    RESET_MSTP_DEV_IDX_EHCI0,
    RESET_MSTP_DEV_IDX_HSUSB,
    RESET_MSTP_DEV_IDX_HSUSB3,
    RESET_MSTP_DEV_IDX_CMM3,
    RESET_MSTP_DEV_IDX_CMM2,
    RESET_MSTP_DEV_IDX_CMM1,
    RESET_MSTP_DEV_IDX_CMM0,
    RESET_MSTP_DEV_IDX_CSI20,
    RESET_MSTP_DEV_IDX_CSI41,
    RESET_MSTP_DEV_IDX_CSI40,
    RESET_MSTP_DEV_IDX_DU3,
    RESET_MSTP_DEV_IDX_DU2,
    RESET_MSTP_DEV_IDX_DU1,
    RESET_MSTP_DEV_IDX_DU0,
    RESET_MSTP_DEV_IDX_LVDS,
    RESET_MSTP_DEV_IDX_HDMI1,
    RESET_MSTP_DEV_IDX_HDMI0,
    RESET_MSTP_DEV_IDX_VIN7,
    RESET_MSTP_DEV_IDX_VIN6,
    RESET_MSTP_DEV_IDX_VIN5,
    RESET_MSTP_DEV_IDX_VIN4,
    RESET_MSTP_DEV_IDX_VIN3,
    RESET_MSTP_DEV_IDX_VIN2,
    RESET_MSTP_DEV_IDX_VIN1,
    RESET_MSTP_DEV_IDX_VIN0,
    RESET_MSTP_DEV_IDX_ETHERAVB,
    RESET_MSTP_DEV_IDX_SATA0,
    RESET_MSTP_DEV_IDX_IMR3,
    RESET_MSTP_DEV_IDX_IMR2,
    RESET_MSTP_DEV_IDX_IMR1,
    RESET_MSTP_DEV_IDX_IMR0,
    RESET_MSTP_DEV_IDX_GPIO7,
    RESET_MSTP_DEV_IDX_GPIO6,
    RESET_MSTP_DEV_IDX_GPIO5,
    RESET_MSTP_DEV_IDX_GPIO4,
    RESET_MSTP_DEV_IDX_GPIO3,
    RESET_MSTP_DEV_IDX_GPIO2,
    RESET_MSTP_DEV_IDX_GPIO1,
    RESET_MSTP_DEV_IDX_GPIO0,
    RESET_MSTP_DEV_IDX_CAN_FD,
    RESET_MSTP_DEV_IDX_CAN_IF1,
    RESET_MSTP_DEV_IDX_CAN_IF0,
    RESET_MSTP_DEV_IDX_I2C6,
    RESET_MSTP_DEV_IDX_I2C5,
    RESET_MSTP_DEV_IDX_I2C_DVFS,
    RESET_MSTP_DEV_IDX_I2C4,
    RESET_MSTP_DEV_IDX_I2C3,
    RESET_MSTP_DEV_IDX_I2C2,
    RESET_MSTP_DEV_IDX_I2C1,
    RESET_MSTP_DEV_IDX_I2C0,
    RESET_MSTP_DEV_IDX_SSI_ALL,
    RESET_MSTP_DEV_IDX_SSI9,
    RESET_MSTP_DEV_IDX_SSI8,
    RESET_MSTP_DEV_IDX_SSI7,
    RESET_MSTP_DEV_IDX_SSI6,
    RESET_MSTP_DEV_IDX_SSI5,
    RESET_MSTP_DEV_IDX_SSI4,
    RESET_MSTP_DEV_IDX_SSI3,
    RESET_MSTP_DEV_IDX_SSI2,
    RESET_MSTP_DEV_IDX_SSI1,
    RESET_MSTP_DEV_IDX_SSI0,
    RESET_MSTP_DEV_IDX_COUNT
};

#endif /* RESET_MSTP_DEVICES_H */
