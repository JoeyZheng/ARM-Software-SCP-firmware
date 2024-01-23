#
# Arm SCP/MCP Software
# Copyright (c) 2020-2023, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#
# Create the firmware target.
#

add_executable(juno-bl2)

# SCP_PLATFORM_VARIANT options:
# - 'BOARD' for building to run on SoC (default)
# - 'FVP' for building to run on an FVP

target_compile_definitions(juno-bl2 PUBLIC -DJUNO_VARIANT_BOARD=0)
target_compile_definitions(juno-bl2 PUBLIC -DJUNO_VARIANT_FVP=1)

if (SCP_PLATFORM_VARIANT STREQUAL "BOARD")
    message(NOTICE "SCP_PLATFORM_VARIANT set to BOARD (juno-bl2)\n")
    target_compile_definitions(juno-bl2
        PUBLIC -DPLATFORM_VARIANT=JUNO_VARIANT_BOARD)

    target_sources(juno-bl2 PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_xrp7724.c")
    target_sources(juno-bl2 PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_pvt.c")
    target_sources(juno-bl2 PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_adc.c")

elseif (SCP_PLATFORM_VARIANT STREQUAL "FVP")
    message(NOTICE "SCP_PLATFORM_VARIANT set to FVP (juno-bl2)\n")
    target_compile_definitions(juno-bl2
        PUBLIC -DPLATFORM_VARIANT=JUNO_VARIANT_FVP)
    
    # config if we are not using DT
    if (NOT SCP_FIRMWARE_DTS_SOURCE)
        target_sources(juno-bl2 PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/config_mock_clock.c")
        target_sources(juno-bl2 PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/config_mock_psu.c")
    endif()

else ()
    message(FATAL_ERROR "Valid platform variant options for Juno are: \n"
                                "BOARD (default) \n"
                                "FVP \n")
endif()

target_include_directories(
    juno-bl2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include"
                    "${CMAKE_CURRENT_SOURCE_DIR}")

# cmake-lint: disable=E1122

target_sources(
    juno-bl2
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_soc_clock_ram.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_dvfs.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_cdcel937.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_hdlcd.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_ddr_phy400.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_dmc400.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_ram.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_i2c.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_reg_sensor.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_psu.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_thermal.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/../src/juno_utils.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/../src/juno_id.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/../src/juno_scmi_clock.c")

# compile DT sources if DT build is enabled, otherwise build the old  way 
if (SCP_FIRMWARE_DTS_SOURCE) 

    # specific Juno DT config files which override some of the standard config built in the modules, 
    # other DT config files are built automatically as part of the modules
    target_sources(
        juno-bl2
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_juno_sds.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_juno_pl011.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_juno_sensor.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_juno_clock.c")

else()

    # files that are replaced by DT versions either in Juno or common
    # framework modules.
    #
    # we allow compile these in if not using DT
    target_sources(
        juno-bl2
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_pl011.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_clock.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_mock_clock.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_power_domain.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_timer.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_system_power.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_mock_psu.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_ppu.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_mhu.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_sds.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_transport.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_sensor.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_scmi_power_domain.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_scmi_system_power.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_scmi_clock.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_scmi_perf.c"    
                "${CMAKE_CURRENT_SOURCE_DIR}/config_scmi.c")
endif()

# select device tree configured modules. Add all non conditional supported here as will
# only build if SCP_FIRMWARE_DTS_SOURCE is set.
# These use the .dts configured variants
# 
# Order is not important for this list
list(APPEND SCP_DT_CONFIG_MODULES_STD "pl011")
list(APPEND SCP_DT_CONFIG_MODULES_DYN "clock")
list(APPEND SCP_DT_CONFIG_MODULES_STD "gtimer")
list(APPEND SCP_DT_CONFIG_MODULES_STD "timer")
list(APPEND SCP_DT_CONFIG_MODULES_STD "juno-ppu")
list(APPEND SCP_DT_CONFIG_MODULES_STD "system-power")
list(APPEND SCP_DT_CONFIG_MODULES_STD "power-domain")
list(APPEND SCP_DT_CONFIG_MODULES_STD "mhu")
list(APPEND SCP_DT_CONFIG_MODULES_STD "scmi")
list(APPEND SCP_DT_CONFIG_MODULES_STD "scmi-clock")
list(APPEND SCP_DT_CONFIG_MODULES_STD "scmi-perf")
list(APPEND SCP_DT_CONFIG_MODULES_STD "scmi-power-domain")
list(APPEND SCP_DT_CONFIG_MODULES_STD "scmi-system-power")
list(APPEND SCP_DT_CONFIG_MODULES_DYN "sds")
list(APPEND SCP_DT_CONFIG_MODULES_DYN "sensor")
list(APPEND SCP_DT_CONFIG_MODULES_STD "transport")
list(APPEND SCP_DT_CONFIG_MODULES_STD "i2c")
list(APPEND SCP_DT_CONFIG_MODULES_STD "dw-apb-i2c")

# choose "BOARD" or "FVP" only DT modules
if (SCP_PLATFORM_VARIANT STREQUAL "BOARD")
    # BOARD DT modules
else()
    # FVP DT modules
    list(APPEND SCP_DT_CONFIG_MODULES_STD "mock-psu")
    list(APPEND SCP_DT_CONFIG_MODULES_STD "mock-clock")
endif()

# add in juno specific headers that are not include by other juno module builds
# to be processed for enums and #defines into the generated DT bindings .h file.
# 
list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/fmw_cmsis.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_clock.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_mmap.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_mhu.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_sds.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/software_mmap.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_scmi.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/system_clock.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/system_mmap.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_power_domain.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_ppu_idx.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_alarm_idx.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../module/juno_cdcel937/include/mod_juno_cdcel937.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../module/juno_hdlcd/include/mod_juno_hdlcd.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../module/juno_soc_clock_ram/include/mod_juno_soc_clock_ram.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../module/juno_xrp7724/include/mod_juno_xrp7724.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../module/juno_pvt/include/mod_juno_pvt.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/../module/juno_adc/include/mod_juno_adc.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/config_power_domain.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/config_mock_psu.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/config_dvfs.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/config_sensor.h"
  )

# create a list of optional .dtsi includes that will be added to the final dt compile
# These are dependent on compile time defines which are generated later,
# so add to define list and file list for later processing
list(APPEND SCP_MODULE_DT_OPTIONS_DEFINE_REQ "BUILD_HAS_SCMI_NOTIFICATIONS")
list(APPEND SCP_MODULE_DT_OPTIONS_FILE_REQ "juno-scp-scmi-opts.dtsi")
#list(APPEND SCP_MODULE_DT_OPTIONS_DEFINE_REQ "BUILD_HAS_NOTIFICATION")
#list(APPEND SCP_MODULE_DT_OPTIONS_FILE_REQ "juno-scp-notification-opts.dtsi")

if (JUNO_BUILD_FULL_SET_SENSORS)
    string(APPEND SCP_MODULE_DT_OPTIONS_DTSI "#include \"juno-scp-sensor-opts.dtsi\"\n")
endif()

if(SCP_ENABLE_DEBUG_UNIT)

    if (NOT SCP_FIRMWARE_DTS_SOURCE)
        target_sources(
            juno-bl2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_debug.c"
                             "${CMAKE_CURRENT_SOURCE_DIR}/config_debug.c")
    endif()
endif()

if(SCP_ENABLE_SCMI_RESET)
    if (NOT SCP_FIRMWARE_DTS_SOURCE)
        target_sources(
            juno-bl2
            PRIVATE
                "${CMAKE_CURRENT_SOURCE_DIR}/config_reset_domain.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_scmi_reset_domain.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/config_juno_reset_domain.c")
    endif()
endif()

if(SCP_ENABLE_STATISTICS)
    if (NOT SCP_FIRMWARE_DTS_SOURCE)
        target_sources(juno-bl2
                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_stats.c")
    endif()
endif()

if(${SCP_ENABLE_RESOURCE_PERMISSIONS})
    target_sources(
        juno-bl2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_resource_perms.c")
endif()

if(SCP_ENABLE_PMI)
    target_sources(
        juno-bl2 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dwt_pmi.c"
                         "${CMAKE_CURRENT_SOURCE_DIR}/config_pmi.c")
endif()

#
# Some of our firmware includes require CMSIS.
#

target_link_libraries(juno-bl2 PUBLIC cmsis::core-m)

#
# We explicitly add the CMSIS include directories to our interfaceinclude
# directories. Each module target adds these include directories totheir own,
# allowing them to include any firmware includes we expose.
#

target_include_directories(
    juno-bl2
    PUBLIC $<TARGET_PROPERTY:cmsis::core-m,INTERFACE_INCLUDE_DIRECTORIES>)

if(SCP_ENABLE_DEBUG_UNIT)
    list(APPEND SCP_MODULES "debug")
    list(APPEND SCP_MODULES "juno-debug")
    if (SCP_FIRMWARE_DTS_SOURCE)
        list(APPEND SCP_DT_CONFIG_MODULES_DYN "juno-debug")
        list(APPEND SCP_DT_CONFIG_MODULES_STD "debug")
        string(APPEND SCP_MODULE_DT_OPTIONS_DTSI "#include \"juno-scp-scmi-debug-opts.dtsi\"\n")
    endif()
endif()

if(SCP_ENABLE_SCMI_RESET)
    list(APPEND SCP_MODULES "reset-domain")
    list(APPEND SCP_MODULES "scmi-reset-domain")
    list(APPEND SCP_MODULES "juno-reset-domain")
    if (SCP_FIRMWARE_DTS_SOURCE)
        list(APPEND SCP_DT_CONFIG_MODULES_STD "reset-domain")
        list(APPEND SCP_DT_CONFIG_MODULES_STD "scmi-reset-domain")
        list(APPEND SCP_DT_CONFIG_MODULES_STD "juno-reset-domain")
        string(APPEND SCP_MODULE_DT_OPTIONS_DTSI "#include \"juno-scp-scmi-reset-opts.dtsi\"\n")
    endif()
endif()

if(SCP_ENABLE_RESOURCE_PERMISSIONS)
    list(APPEND SCP_MODULES "resource-perms")
endif()

if(SCP_ENABLE_STATISTICS)
    list(APPEND SCP_MODULES "statistics")
    if (SCP_FIRMWARE_DTS_SOURCE)
        list(APPEND SCP_DT_CONFIG_MODULES_STD "statistics")
    endif()
endif()

set(SCP_MODULES ${SCP_MODULES} PARENT_SCOPE)
set(SCP_MODULE_PATHS ${SCP_MODULE_PATHS} PARENT_SCOPE)


# push all the DT variables down to parent scope 
set(SCP_DT_CONFIG_MODULES_DYN ${SCP_DT_CONFIG_MODULES_DYN} PARENT_SCOPE)
set(SCP_DT_CONFIG_MODULES_STD ${SCP_DT_CONFIG_MODULES_STD} PARENT_SCOPE)
set(SCP_DT_BIND_H_GEN_FROM_INCL ${SCP_DT_BIND_H_GEN_FROM_INCL} PARENT_SCOPE)
set(SCP_MODULE_DT_OPTIONS_DEFINE_REQ ${SCP_MODULE_DT_OPTIONS_DEFINE_REQ} PARENT_SCOPE)
set(SCP_MODULE_DT_OPTIONS_FILE_REQ ${SCP_MODULE_DT_OPTIONS_FILE_REQ} PARENT_SCOPE)
set(SCP_MODULE_DT_OPTIONS_DTSI ${SCP_MODULE_DT_OPTIONS_DTSI} PARENT_SCOPE)
