#
# Arm SCP/MCP Software
# Copyright (c) 2021-2023, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

add_library(${SCP_MODULE_TARGET} SCP_MODULE)

target_include_directories(${SCP_MODULE_TARGET}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_sources(
    ${SCP_MODULE_TARGET}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/mod_power_domain.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/power_domain_utils.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/power_domain_state_checks.c")

if(SCP_MODULE IN_LIST SCP_DT_CONFIG_MODULES_ALL)
    target_sources(${SCP_MODULE_TARGET}
                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/config_dt_power_domain.c")

    # add header file to list used to generate defines for dt include 
    list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/include/mod_power_domain.h")
    set(SCP_DT_BIND_H_GEN_FROM_INCL "${SCP_DT_BIND_H_GEN_FROM_INCL}" PARENT_SCOPE)
   

endif()

if(SCP_ENABLE_NOTIFICATIONS)
    target_sources(${SCP_MODULE_TARGET}
                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/power_domain_notifications.c")
endif()
