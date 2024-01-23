#
# Arm SCP/MCP Software
# Copyright (c) 2023, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

add_library(${SCP_MODULE_TARGET} SCP_MODULE)

target_include_directories(${SCP_MODULE_TARGET}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_sources(${SCP_MODULE_TARGET}
               PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/mod_transport.c")

if(SCP_MODULE IN_LIST SCP_DT_CONFIG_MODULES_ALL)
    target_sources(${SCP_MODULE_TARGET}
                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/config_dt_transport.c")
           
    # add header file to list used to generate defines for dt include 
    list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/include/mod_transport.h")
    set(SCP_DT_BIND_H_GEN_FROM_INCL "${SCP_DT_BIND_H_GEN_FROM_INCL}" PARENT_SCOPE)
endif()
           
if("scmi" IN_LIST SCP_MODULES)
    target_link_libraries(${SCP_MODULE_TARGET} PRIVATE module-scmi)
endif()

if("power-domain" IN_LIST SCP_MODULES)
    target_link_libraries(${SCP_MODULE_TARGET} PRIVATE module-power-domain)
endif()

if(BUILD_HAS_MOD_TRANSPORT_FC)
    target_compile_definitions(${SCP_MODULE_TARGET}
        PUBLIC "BUILD_HAS_MOD_TRANSPORT_FC")
endif()
