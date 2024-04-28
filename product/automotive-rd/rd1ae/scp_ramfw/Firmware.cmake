#
# Arm SCP/MCP Software
# Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#
# Configure the build system.
#

set(SCP_FIRMWARE "rd1ae-bl2")

set(SCP_FIRMWARE_TARGET "rd1ae-bl2")

set(SCP_TOOLCHAIN_INIT "GNU")

set(SCP_GENERATE_FLAT_BINARY_INIT TRUE)

set(SCP_ENABLE_NOTIFICATIONS_INIT TRUE)

# Disable Interprocedural optimization
set(SCP_ENABLE_IPO_INIT FALSE)

set(SCP_ARCHITECTURE "arm-m")

set(SCP_ENABLE_NEWLIB_NANO FALSE)

set(SCP_ENABLE_OUTBAND_MSG_SUPPORT TRUE)

list(PREPEND SCP_MODULE_PATHS
     "${CMAKE_CURRENT_LIST_DIR}/../module/system_pik")


# The order of the modules in the following list is the order in which the
# modules are initialized, bound, started during the pre-runtime phase.
# Any change in the order will cause firmware initialization errors.
list(APPEND SCP_MODULES "armv7m-mpu")
list(APPEND SCP_MODULES "pl011")
list(APPEND SCP_MODULES "system-pik")
list(APPEND SCP_MODULES "ppu-v1")
list(APPEND SCP_MODULES "system-pll")
list(APPEND SCP_MODULES "pik-clock")
list(APPEND SCP_MODULES "gtimer")
list(APPEND SCP_MODULES "timer")
list(APPEND SCP_MODULES "mhu3")
list(APPEND SCP_MODULES "transport")
list(APPEND SCP_MODULES "pcid")
list(APPEND SCP_MODULES "sid")
list(APPEND SCP_MODULES "system-info")
