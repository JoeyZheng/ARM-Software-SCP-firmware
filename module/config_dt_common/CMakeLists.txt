#
# Arm SCP/MCP Software
# Copyright (c) 2021-2024, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Framework Modules - common CMakelists to add compile elements for config_dt module support

# adds in the config_dt_<mod> compilation for individual modules if in list
# moved from individual module directories on maintainers request.

# iterate through the lists of modules and targets

# if module matches config dt list then check each of the specialisations

foreach(SCP_MODULE IN LISTS SCP_VALID_MODULES)

    # We have the module, so grab its target

    list(FIND SCP_VALID_MODULES "${SCP_MODULE}" SCP_MODULE_IDX)
    list(GET SCP_VALID_MODULE_TARGETS ${SCP_MODULE_IDX} SCP_MODULE_TARGET)
    list(GET SCP_VALID_MODULE_SOURCE_DIRS ${SCP_MODULE_IDX}
         SCP_MODULE_SOURCE_DIR)

    # at this point SCP_MODULE and SCP_MODULE_TARGET are defined for a module
    # since we can't use the natural method of putting this code in the individual module CMakelists.txt files,
    # must check each one individually here

    # first check if the module should have a dt config at all
    if(${SCP_MODULE} IN_LIST SCP_DT_CONFIG_MODULES_ALL)

        set(dt_module_found FALSE)

        # now check against each of the configured modules.
        message(VERBOSE "looking for DT Module ${SCP_MODULE}\n"
                         "module target: ${SCP_MODULE_TARGET}\n"
                         "module source dir: ${SCP_MODULE_SOURCE_DIR}\n")                         

        # check for bootloader module
        if("${SCP_MODULE}" STREQUAL "bootloader")
            set(dt_module_found TRUE)
            
            target_sources(${SCP_MODULE_TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_bootloader.c")

            # do not set SCP_DT_BIND_H_GEN_FROM_INCL - no req enums or defines in header
        endif()
        # end bootloader

        # check for clock module
        if("${SCP_MODULE}" STREQUAL "clock")
            set(dt_module_found TRUE)
            
            target_sources(${SCP_MODULE_TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_clock.c")

             # add header file to list used to generate defines for dt include 
             list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../clock/include/mod_clock.h")
        endif()
        # end clock 

        # check module gtimer
        if("${SCP_MODULE}" STREQUAL "gtimer")
            set(dt_module_found TRUE)
            
            target_sources(${SCP_MODULE_TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_gtimer.c")

            # do not set SCP_DT_BIND_H_GEN_FROM_INCL - no req enums or defines in header
        endif()
        # end gtimer

        # check module mock_clock 
        if("${SCP_MODULE}" STREQUAL "mock-clock")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_mock_clock.c")

            # add header file to list used to generate defines for dt include 
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../mock_clock/include/mod_mock_clock.h")
       
        endif()
        # end mock_clock
        
        # check module mock-psu 
        if("${SCP_MODULE}" STREQUAL "mock-psu")
            set(dt_module_found TRUE)

            target_sources(${SCP_MODULE_TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_mock_psu.c")

            # add header file to list used to generate defines for dt include 
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../mock_psu/include/mod_mock_psu.h")           
       
        endif()
        # end

        # check module pl011
        if("${SCP_MODULE}" STREQUAL "pl011")
             set(dt_module_found TRUE)
             target_sources(${SCP_MODULE_TARGET}
                    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_pl011.c")

              # do not set SCP_DT_BIND_H_GEN_FROM_INCL - no req emums or defines in header             
       
        endif()
        # end pl011

        # check module power-domain 
        if("${SCP_MODULE}" STREQUAL "power-domain")
             set(dt_module_found TRUE)
             target_sources(${SCP_MODULE_TARGET}
                            PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_power_domain.c")

              # add header file to list used to generate defines for dt include 
              list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../power_domain/include/mod_power_domain.h")
       
        endif()
        # end power-domain
        
        # check module sds
        if("${SCP_MODULE}" STREQUAL "sds")
             set(dt_module_found TRUE)
             target_sources(${SCP_MODULE_TARGET}
                            PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_sds.c")

             # add header file to list used to generate defines for dt include 
             list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../sds/include/mod_sds.h")
       
        endif()
        # end sds

        # check module system-power
        if("${SCP_MODULE}" STREQUAL "system-power")
             set(dt_module_found TRUE)
             target_sources(${SCP_MODULE_TARGET}
                            PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_system_power.c")
    
             # add header file to list used to generate defines for dt include 
             list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../system_power/include/mod_system_power.h")
       
        endif()
        # end system-power
        
        # check module timer 
        if("${SCP_MODULE}" STREQUAL "timer")
             set(dt_module_found TRUE)
             target_sources(${SCP_MODULE_TARGET}
                            PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_timer.c")

             # add header file to list used to generate defines for dt include 
             list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../timer/include/mod_timer.h")
       
        endif()
        # end

        # check module scmi
        if("${SCP_MODULE}" STREQUAL "scmi")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_scmi.c")

            # add header file to list used to generate defines for dt include
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../scmi/include/mod_scmi.h")
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../scmi/include/mod_scmi_std.h")
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../scmi/include/internal/scmi.h")
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../transport/include/mod_transport.h")
       
        endif()
        # end scmi
        
        # check module debug
        if("${SCP_MODULE}" STREQUAL "debug")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_debug.c")
       
        endif()
        # end debug

        # check module mhu
        if("${SCP_MODULE}" STREQUAL "mhu")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_mhu.c")
       
        endif()
        # end mhu
        
        # check module reset-domain 
        if("${SCP_MODULE}" STREQUAL "reset-domain")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_reset_domain.c")

            # add header file to list used to generate defines for dt include 
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../reset_domain/include/mod_reset_domain.h")
       
        endif()
        # end reset-domain
        
        # check module scmi-clock
        if("${SCP_MODULE}" STREQUAL "scmi-clock")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_scmi_clock.c")
           
            # add header file to list used to generate defines for dt include 
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../scmi_clock/include/mod_scmi_clock.h")
       
        endif()
        # end scmi-clock
        
        # check module scmi-perf 
        if("${SCP_MODULE}" STREQUAL "scmi-perf")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_scmi_perf.c")

            # add header file to list used to generate defines for dt include 
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../scmi_perf/include/mod_scmi_perf.h")
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../scmi_perf/include/internal/scmi_perf.h")
        
        endif()
        # end scmi-perf
        
        # check module scmi-power-domain 
        if("${SCP_MODULE}" STREQUAL "scmi-power-domain")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_scmi_power_domain.c")
       
        endif()
        # end scmi-power-domain
        
        # check module scmi-reset-domain 
        if("${SCP_MODULE}" STREQUAL "scmi-reset-domain")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_scmi_reset_domain.c")

            # add header file to list used to generate defines for dt include 
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../scmi_reset_domain/include/mod_scmi_reset_domain.h")
       
        endif()
        # end scmi-reset-domain
        
        # check module scmi-system-power
        if("${SCP_MODULE}" STREQUAL "scmi-system-power")
            set(dt_module_found TRUE)
            target_sources(${SCP_MODULE_TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_scmi_system_power.c")

            # add header file to list used to generate defines for dt include 
            list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../scmi_system_power/include/mod_scmi_system_power.h")
       
        endif()
        # end scmi-system-power
        
        # add more modules here as required.

        # check if the dt module listed has actually had code set up
        if (dt_module_found)
           message(VERBOSE "Framework module DT support found ${SCP_MODULE}\n")
           list(APPEND SCP_DT_MODULE_SUPPORT_FOUND "${SCP_MODULE}")
        endif()

    # end of check for module in DT list
    endif()
  
endforeach()

# push the variables to parent scope
set(SCP_DT_BIND_H_GEN_FROM_INCL "${SCP_DT_BIND_H_GEN_FROM_INCL}" PARENT_SCOPE)
set(SCP_DT_BIND_H_GEN_DEFS      "${SCP_DT_BIND_H_GEN_DEFS}"      PARENT_SCOPE)
set(SCP_DT_MODULE_SUPPORT_FOUND "${SCP_DT_MODULE_SUPPORT_FOUND}" PARENT_SCOPE)
