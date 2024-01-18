#!/bin/bash
# build juno fw

TARGETS="all"
OPERATION="full"
VERBOSE=
MODE=
VALID_TARGETS="scp_ramfw scp_romfw scp_romfw_bypass"
USE_DT_BUILD="USE_DT_BUILD=1"
EX_CFG_ARGS=

# multiple options suitable for juno build - add here as cannot pass as string on command line
JUNOOPTS=" EXTRA_CONFIG_ARGS+=-DSCP_ENABLE_SCMI_PERF_FAST_CHANNELS=TRUE EXTRA_CONFIG_ARGS+=-DSCP_ENABLE_STATISTICS=TRUE"
JFULLSENSORS=""
OPTS_SEL=
PLAT_VAR=

RESET_OPTS=" EXTRA_CONFIG_ARGS+=-DSCP_ENABLE_SCMI_RESET=TRUE"
RESET_OPTS+=" EXTRA_CONFIG_ARGS+=-DSCP_ENABLE_NOTIFICATIONS=TRUE"
RESET_OPTS+=" EXTRA_CONFIG_ARGS+=-DSCP_ENABLE_SCMI_NOTIFICATIONS=TRUE"

DEBUG_OPTS=" EXTRA_CONFIG_ARGS+=-DSCP_ENABLE_DEBUG_UNIT=TRUE"

print_help() {
    echo "CMake builder for juno"
    echo "Defaults to all targets, full build"
    echo "options:"
    echo "-v : verbose build output"
    echo "-t : select specific target - one of ${VALID_TARGETS}"
    echo "-d : build debug mode"
    echo "-z : supress DT build flow and use old modules"
    echo "-o : operation is one of:"
    echo "       generate - generate only"
    echo "       cpp_only - stop after C pre-processor to check macro generation"
    echo "       clean    - clean build area"
    echo "-j : use juno opts : ${JUNOOPTS}"
    echo "-k : use juno full set sensors"
    echo "-f : build fvp variant"
    echo "-s : opts selection one of:"
    echo "     reset - enable reset modules (${RESET_OPTS})"
    echo "     debug - enable debug modules (${DEBUG_OPTS})"
    echo "     all  - enable all optional modules"
    exit 1
}

check_target () {
    for valid in ${VALID_TARGETS}
    do
        if [ "$1" == "${valid}" ]; then
            return
        fi
    done
    echo "Invalid target $1"
    print_help
}

while getopts o:t:e:s:vdzhjkf flag
do
    case "${flag}" in
        o) OPERATION=${OPTARG};;
        t) TARGETS=${OPTARG};;
        v) VERBOSE="V=y";;
        d) MODE="MODE=debug";;
        z) USE_DT_BUILD=;;
        e) EX_CFG_ARG="EXTRA_CONFIG_ARGS+="${OPTARG};;
        j) USE_JOPTS=${JUNOOPTS};;
        k) JFULLSENSORS=" EXTRA_CONFIG_ARGS+=-DJUNO_BUILD_FULL_SET_SENSORS";;
        s) OPTS_SEL=${OPTARG};;
        f) PLAT_VAR="PLATFORM_VARIANT=FVP";;
        h) print_help;;
    esac
done

if [ "${TARGETS}" == "all" ]; then
    TARGETS="${VALID_TARGETS}"
fi


if [ "${OPTS_SEL}" == "reset" ]; then
    USE_JOPTS+=" ${RESET_OPTS}"
fi

if [ "${OPTS_SEL}" == "debug" ]; then
    USE_JOPTS+=" ${DEBUG_OPTS}"
fi

if [ "${OPTS_SEL}" == "all" ]; then
    USE_JOPTS+=" ${DEBUG_OPTS} ${RESET_OPTS}"
fi

USE_JOPTS+=" ${JFULLSENSORS}"
    
for target in ${TARGETS} 
do
    echo "Building: Op=${OPERATION}, target=${target}, Plat-var=${PLAT_VAR} Verbose=${VERBOSE}, DT build option=${USE_DT_BUILD} ExCFG=${EX_CFG_ARG} ${USE_JOPTS}"
    check_target ${target}
    if [ "${OPERATION}" == "generate" ]; then
        make -f Makefile.cmake PRODUCT=juno BUILD_SYSTEM=Make BUILD_TARGET=${target} ${PLAT_VAR} ${EX_CFG_ARG} ${USE_JOPTS} ${MODE} ${USE_DT_BUILD} generate $VERBOSE 
    elif [  "${OPERATION}" == "cpp_only"  ]; then
        make -f Makefile.cmake PRODUCT=juno BUILD_SYSTEM=Make BUILD_TARGET=${target} ${PLAT_VAR} ${EX_CFG_ARG} ${USE_JOPTS} ${MODE} ${USE_DT_BUILD} CPP_HALT=1 $VERBOSE
    elif [  "${OPERATION}" == "clean" ]; then
        make -f Makefile.cmake clean
        # clean removes the full build dir - only do once
        break
    elif [  "${OPERATION}" == "full" ]; then
        make -f Makefile.cmake PRODUCT=juno BUILD_SYSTEM=Make ${PLAT_VAR} ${EX_CFG_ARG} ${USE_JOPTS} ${MODE} ${USE_DT_BUILD} BUILD_TARGET=${target} $VERBOSE
    else
        echo "Invalid Operation specified"
        print_help
    fi
done

exit 0

        
        
    
