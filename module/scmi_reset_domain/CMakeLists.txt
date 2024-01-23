#
# Arm SCP/MCP Software
# Copyright (c) 2021, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

add_library(${SCP_MODULE_TARGET} SCP_MODULE)

target_include_directories(${SCP_MODULE_TARGET}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_sources(
    ${SCP_MODULE_TARGET}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/mod_scmi_reset_domain.c")

if(SCP_MODULE IN_LIST SCP_DT_CONFIG_MODULES_ALL)
    target_sources(${SCP_MODULE_TARGET}
                    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/config_dt_scmi_reset_domain.c")

    # add header file to list used to generate defines for dt include 
    list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/include/mod_scmi_reset_domain.h")
    set(SCP_DT_BIND_H_GEN_FROM_INCL "${SCP_DT_BIND_H_GEN_FROM_INCL}" PARENT_SCOPE)
endif()

if("reset-domain" IN_LIST SCP_MODULES)
    target_link_libraries(${SCP_MODULE_TARGET} PRIVATE module-reset-domain)
endif()

target_link_libraries(${SCP_MODULE_TARGET} PRIVATE module-scmi)

if("resource-perms" IN_LIST SCP_MODULES)
    target_link_libraries(${SCP_MODULE_TARGET} PRIVATE module-resource-perms)
endif()
