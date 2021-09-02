#
# Arm SCP/MCP Software
# Copyright (c) 2021, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#
# Configure the build system.
#

set(SCP_FIRMWARE "synquacer-bl2")

set(SCP_FIRMWARE_TARGET "synquacer-bl2")

set(SCP_TOOLCHAIN_INIT "GNU")

set(SCP_GENERATE_FLAT_BINARY_INIT TRUE)

set(SCP_ENABLE_MULTITHREADING_INIT FALSE)

set(SCP_ENABLE_NOTIFICATIONS_INIT TRUE)

set(SCP_ENABLE_IPO_INIT FALSE)

set(SCP_ARCHITECTURE "armv7-m")

list(PREPEND SCP_MODULE_PATHS
     "${CMAKE_CURRENT_LIST_DIR}/../module/scmi_vendor_ext")
list(PREPEND SCP_MODULE_PATHS
     "${CMAKE_CURRENT_LIST_DIR}/../module/synquacer_memc")
list(PREPEND SCP_MODULE_PATHS "${CMAKE_CURRENT_LIST_DIR}/../module/f_i2c")
list(PREPEND SCP_MODULE_PATHS "${CMAKE_CURRENT_LIST_DIR}/../module/f_uart3")
list(PREPEND SCP_MODULE_PATHS "${CMAKE_CURRENT_LIST_DIR}/../module/hsspi")
list(PREPEND SCP_MODULE_PATHS "${CMAKE_CURRENT_LIST_DIR}/../module/ccn512")
list(PREPEND SCP_MODULE_PATHS
     "${CMAKE_CURRENT_LIST_DIR}/../module/ppu_v0_synquacer")
list(PREPEND SCP_MODULE_PATHS
     "${CMAKE_CURRENT_LIST_DIR}/../module/synquacer_system")

# The order of the modules in the following list is the order in which the
# modules are initialized, bound, started during the pre-runtime phase.
# any change in the order will cause firmware initialization errors.

list(APPEND SCP_MODULES "armv7m-mpu")
list(APPEND SCP_MODULES "f-uart3")
list(APPEND SCP_MODULES "gtimer")
list(APPEND SCP_MODULES "timer")
list(APPEND SCP_MODULES "ppu-v0-synquacer")
list(APPEND SCP_MODULES "system-power")
list(APPEND SCP_MODULES "power-domain")
list(APPEND SCP_MODULES "clock")
list(APPEND SCP_MODULES "synquacer-system")
list(APPEND SCP_MODULES "pik-clock")
list(APPEND SCP_MODULES "css-clock")
list(APPEND SCP_MODULES "ccn512")
list(APPEND SCP_MODULES "f-i2c")
list(APPEND SCP_MODULES "hsspi")
list(APPEND SCP_MODULES "synquacer-memc")
list(APPEND SCP_MODULES "mhu")
list(APPEND SCP_MODULES "smt")
list(APPEND SCP_MODULES "scmi")
list(APPEND SCP_MODULES "scmi-power-domain")
list(APPEND SCP_MODULES "scmi-system-power")
list(APPEND SCP_MODULES "scmi-apcore")
list(APPEND SCP_MODULES "scmi-vendor-ext")

add_compile_definitions(PUBLIC HAS_RTOS PCIE_FILTER_BUS0_TYPE0_CONFIG
                        ENABLE_OPTEE SET_PCIE_NON_SECURE)
