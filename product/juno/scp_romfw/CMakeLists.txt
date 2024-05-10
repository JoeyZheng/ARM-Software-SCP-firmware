#
# Arm SCP/MCP Software
# Copyright (c) 2021-2023, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#
# Create the firmware target.
#

add_executable(juno-bl1)

if (NOT SCP_PLATFORM_VARIANT STREQUAL "BOARD" AND
    NOT SCP_PLATFORM_VARIANT STREQUAL "FVP")
        message(FATAL_ERROR "Valid platform variant options for Juno are: \n"
                                    "BOARD (default) \n"
                                    "FVP \n")
endif()

target_include_directories(
    juno-bl1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include"
                    "${CMAKE_CURRENT_SOURCE_DIR}")

# cmake-lint: disable=E1122

target_sources(
    juno-bl1
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_soc_clock.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/../src/juno_utils.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/../src/juno_id.c")

# compile DT sources if DT build is enabled, otherwise build the old  way 
if (SCP_FIRMWARE_DTS_SOURCE) 

    # specific Juno DT config files, other DT config files are built automatically
    # as part of the modules
    target_sources(
        juno-bl1
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_juno_sds.c")

else()

    # files that are replaced by DT versions either in Juno or common
    # framework modules.
    #
    # for now we allow option to build the firmware with the deprecated module
    # config methods.
    target_sources(
        juno-bl1
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_bootloader.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_clock.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_rom.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_timer.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_sds.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_ppu.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_rom.c")
endif()

# select device tree configured modules
# These use the .dts configured variants
# Order is not important for this list
list(APPEND SCP_DT_CONFIG_MODULES_STD "juno-ppu")
list(APPEND SCP_DT_CONFIG_MODULES_STD "juno-rom")
list(APPEND SCP_DT_CONFIG_MODULES_STD "clock")
list(APPEND SCP_DT_CONFIG_MODULES_STD "gtimer")
list(APPEND SCP_DT_CONFIG_MODULES_DYN "sds")
list(APPEND SCP_DT_CONFIG_MODULES_STD "bootloader")

# add in juno specific headers for device tree bindings processing,
# that are not include by other juno module builds
list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_ppu_idx.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_sds.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/software_mmap.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/scp_mmap.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/system_clock.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/system_mmap.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/fmw_cmsis.h"
    )

# create a list of optional .dtsi includes that will be added to the final dt compile
# These are dependent on compile time defines which are generated later,
# so add to define list and file list for later processing
list(APPEND SCP_MODULE_DT_OPTIONS_DEFINE_REQ "BUILD_MODE_DEBUG")
list(APPEND SCP_MODULE_DT_OPTIONS_FILE_REQ "juno-scp-romfw-debug-opts.dtsi")


#
# Some of our firmware includes require CMSIS.
#

target_link_libraries(juno-bl1 PUBLIC cmsis::core-m)

#
# We explicitly add the CMSIS include directories to our interfaceinclude
# directories. Each module target adds these include directories totheir own,
# allowing them to include any firmware includes we expose.
#

target_include_directories(
    juno-bl1
    PUBLIC $<TARGET_PROPERTY:cmsis::core-m,INTERFACE_INCLUDE_DIRECTORIES>)

# ensure that the juno module config_dt code is processed
if (SCP_FIRMWARE_DTS_SOURCE)
   set(SCP_DT_PRODUCT_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../module/config_dt_common")
endif()

# ensure include lists pushed to parent scope
set(SCP_DT_PRODUCT_MODULE_DIR ${SCP_DT_PRODUCT_MODULE_DIR} PARENT_SCOPE)
set(SCP_DT_CONFIG_MODULES_DYN ${SCP_DT_CONFIG_MODULES_DYN} PARENT_SCOPE)
set(SCP_DT_CONFIG_MODULES_STD ${SCP_DT_CONFIG_MODULES_STD} PARENT_SCOPE)
set(SCP_DT_BIND_H_GEN_FROM_INCL ${SCP_DT_BIND_H_GEN_FROM_INCL} PARENT_SCOPE)
set(SCP_MODULE_DT_OPTIONS_DEFINE_REQ ${SCP_MODULE_DT_OPTIONS_DEFINE_REQ} PARENT_SCOPE)
set(SCP_MODULE_DT_OPTIONS_FILE_REQ ${SCP_MODULE_DT_OPTIONS_FILE_REQ} PARENT_SCOPE)
