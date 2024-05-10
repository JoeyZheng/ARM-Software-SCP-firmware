# Device Tree Configuration Support 

The cmake build system for the SCP firmware has been updated to allow for
modules to be configured using device tree definitions.

To achieve this a number of changes have been made, to integrate some device
tree reading tools from the Zephyr project into the build processes of the SCP
firmware, alongside changes to the build system and modules to optionally
use device tree to populate configuration files.

At present this is a hybrid configuation build process allowing the mixing of modules configured
using the device tree and those configured using directly written C structure tables.

Device tree support is a build time process only.  There is no support for accessing device tree
elements at runtime, due to the impracticalities of loading potentially large binary device tre
objects and code to interrogate them into small memory footprint devices.

## Device tree processing outline

The device tree processing converts device tree files into a set of C macros representing
the individual data values in the device tree nodes and attributes. These macros can then
be used in C code to initialise variables use to configure the SCP system.

The device tree usage is therefore a **build** time process. This happens during the CMake
generation process, where the header files containing the C macros are formed and copied into the
correct locations for use during the product compile time phase.

## Device Tree in SCP

SCP modules all contain configuration tables - a set of statically defined structures that are
written for each module used in each target in each product. Therefore for a common module such as
'sds', used by juno, which has 3 targets, there will be a config_sds.c source file in each of the
target directories.

The device tree support is designed to populate these tables using the device tree information.
As these tables contain both hardware and software configuration, this will be reflected in the
device tree support.

Further, for common modules, there will be a common config_dt_<module>.c file, filled by
the device tree information, and removing the need for multiple config source files for the
same module.


## SCP Build System Changes

The SCP cmake build system has been adjusted to integrate new tooling to process the
device tree source files and allow use of the data in the configuration code.


### Imports From Zephyr

The main Zephyr python based device tree processing library is imported into the SCP
project at the location ` ./contrib/python-devicetree/src/devicetree`

Header files containing additional macros to help with processing the generated elements
can be found at:

- `framework/include/dt_sys/util*` : files copied direct from the Zephyr project
- `framework/include/fwk_devicetree.h` : Main macro API file for using the generated DT data.
  File adapted from the Zephyr file, with minor changes to remove Zephyr specfics.

A modified version of the Zephyr DT reading script has been developed (`tools/dts/dt_gen_config.py`).
This has many of the same features as the original script with necessary adjustments for the SCP
firmware build process.


This script generates a set of complex macros representing DT nodes and attributes.
These are then interpreted using the macros in `fwk_devicetree.h` to provide static values
in configuration data tables.

Further common build sources from Zephyr are added to `dts/bindings`, `dts/include` and `dts/misc`.
These consist of some baseline `.dtsi` files and initial `.yaml` bindings that are re-used.

While the device tree source files are designed to be compiled correctly by the standard device tree compiler,
the Zephyr .yaml for device tree is a slightly custom format and does not currently follow all of the
dt-schema used by the linux kernel and main device tree project. This is likely to  be addressed in future.

### Additional Python Pre-Requisites

The following additional pre-requisites are required for the full DT build flow to work:-

1) pcpp - python based C pre processor. Can be installed from pypi. (https://pypi.org/project/pcpp/)

2) cxxheaderparser - python based processor for C headers. Fetch from
https://github.com/robotpy/cxxheaderparser and run the setup to generate the required version file.

The DT build process will check that all pre-requisites (pcpp, cxxheaderparser and devicetree)  are
present before starting processing.


### The Device tree build flow

#### Updates to the base build process

The project root `CMakelists.txt` file uses a number of new global CMake variables which are used to trigger DT generation and module support.

- `SCP_DT_CONFIG_MODULES_STD` : list of  modules to be configured with DT, using a static data table accessor.
- `SCP_DT_CONFIG_MODULES_DYN` : list of modules to be configured with DT, using a dynamic accessor allowing runtime configuration table adjustments.
- `SCP_DT_CONFIG_MODULES_ALL` : combination of the above two variables, automatically created..
- `SCP_FIRMWARE_DTS_SOURCE`   : The main product .dts source file
- `SCP_FIRMWARE_DTS_INCLUDE`  : Include paths for the .dts includes - may be .h or .dtsi files

In this `CMakelists.txt` file the CMake input variable `SCP_USE_DT_BUILD` is used to control setting `SCP_FIRMWARE_DTS_SOURCE`. If `SCP_USE_DT_BUILD` is set, then `SCP_FIRMWARE_DTS_SOURCE` will be set if supported in the product.

Once `SCP_FIRMWARE_DTS_SOURCE` is set, the root `CMakelists.txt` will include the `\cmake\dts.cmake` file, responsible for running the DTS build. Setting the `SCP_FIRMWARE_DTS_SOURCE` variable will also trigger the DT build flow in any modules that are selected to use the DT in the product.

The framework `\framework\CMakelists.txt` file is updated to use the DT module variables and for each module listed in SCP_DT_CONFIG_MODULES_ALL adjusts the generated  `config_<module>` variables to be `config_dt_<module>` variables in the `fwk_module_list.c` file.

The DT build generation code also creates the `fwk_dt_config_dyn.h` `fwk_dt_mod_idx_gen.h` and `fwk-dt-options.dtsi` files, used in the DT compile phase. A DT specific variable `SCP_DT_BIND_H_GEN_DEFS` is also created with the a set of generated defines BUILD_HAS_MOD_... to be passed to the DT build phase.

- `fwk_dt_config_dyn.h` : contains callbacks for modules listed in the `SCP_DT_CONFIG_MODULES_DYN` list. This allows common modules used in the product to have the configuration tables adjusted at runtime in product specific code.
- `fwk_dt_mod_idx_gen.h` : set of #define values of the selected module indexes for inclusion in the DT source files.
- `fwk-dt-options.dtsi` : generated optional .dtsi includes - allows device tree includes to depend on build defines for optional configuration elements.

e.g. If a module such as pl011 is selected in the SCP_DT_CONFIG_MODULES_DYN list then callbacks are generated in 
`fwk_dt_config_dyn.h` as:

    #define FWK_MODULE_GEN_DYNAMIC_PL011
    extern const struct fwk_element *dyn_get_element_table_pl011(fwk_id_t module_id);
    extern const struct fwk_element *_static_get_element_table_pl011();

The static callback is always implemented in the module source `./module/pl011/config_dt_pl011.c` :

    /* access data table from dynamic element callback */
    #ifdef FWK_MODULE_GEN_DYNAMIC_PL011 
    const struct fwk_element *_static_get_element_table_pl011() {
        return pl011_element_table;
    }
    #endif

    /* config structure */
    struct fwk_module_config config_dt_pl011 = {
        #ifdef FWK_MODULE_GEN_DYNAMIC_PL011
        .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dyn_get_element_table_pl011),
        #else
        .elements = FWK_MODULE_STATIC_ELEMENTS_PTR(pl011_element_table),
        #endif
    };


and the product must then implement the dynamic part of the callback (e.g. in ` ./product/juno/scp_ramfw/config_dt_juno_pl011.c`):

    /* example dynamic module config table implementation function */
    #ifdef FWK_MODULE_GEN_DYNAMIC_PL011
    const struct fwk_element *dyn_get_element_table_pl011(fwk_id_t module_id)
    {
        struct fwk_element *table = _static_get_element_table_pl011();

        /* do some work on the table  */
        
        return table;
    }
    #endif

In this way products can alter module element tables at runtime, even if the module is a common library module.

#### The `dts.cmake` file

This sets up and executes all the operations needed to compile and generate the header files used to access device tree values in the C source files.

The file sets up the various include source, and generated file destination paths. There are 4 key operations.

1. Run the `dt_check_py_modules.py` script. Ensures that the rest of the python scripts can be correctly executed.

2. Run the `dt_gen_enum_defines.py` script. This takes a list of .h include files and extract all the #define expressions, and converts any enum definitions into #define expressions to allow inclusion into the .dts/.dtsi files. This allows the same values used in the C code, to be used as values in the device tree source. The output of this file is the `fwk_dt_gen_bindings.h` file, which appears in the `\build\<product>/.../framework/include` directory for inclusion in the later phases.

3. Runs the C preprocessor on the .dti / .dtsi files, plus and empty.c file, stopping at the end of the pre-processing phase. This combines all the .dts / .dtsi / .h files in the devicetree source in a pre-processed output needed for the device tree script to understand. This pre-processing evaluates the #defines from the `fwk_dt_gen_bindings.h` file to create an intermediate .dts output that contains no unresolved values.

4. Run the `dt_gen_config.py` script on the output of the previous command. This generates the set of macros based on the device tree into the `fwk_dt_config.h` file. This allows the configuration code to use the DT framework macros to extract devicetree values based on specific nodes and attributes.


### Enabling DT in the product

#### Product `Firmware.cmake`

The product `Firmware.cmake` is used to set the devicetree source file path variable `SCP_FIRMWARE_DTS_SOURCE`. This value being set will trigger DT build flow for the product, and any modules selected to use DT build flow in the product.

e.g. Fragment from the juno scp_ramfw Firmware.cmake file:

    # setup Device Tree sources 
    # the root source for this firmware variant
    set(SCP_FIRMWARE_DTS_SOURCE "${CMAKE_CURRENT_LIST_DIR}/juno-scp_ramfw.dts")

    # the includes needed:
    # the current include dir
    list(PREPEND SCP_FIRMWARE_DTS_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/../include")
    # the dts/include/juno specific .dtsi files for this product
    list(PREPEND SCP_FIRMWARE_DTS_INCLUDE "${CMAKE_SOURCE_DIR}/dts/include/juno")

By convention a .dts file will appear in the product directory. This will generally be very simple, including .dtsi include files as the main product device tree source. Any includes will be present in the  `/dts/include` and `/dts/include/<product>` directories.

e.g. the `juno-scp_ramfw.dts` file:

     /* base SCP framework - must be first include */
     #include "scp-fwk.dtsi"

     /* device tree file for Juno RAM SCP firmware, BOARD platform variant */
     #include "juno-scp-base.dtsi"
     #include "juno-scp-ramfw-base.dtsi"
     #include "juno-scp-ramfw-board.dtsi"
     #include "juno-scp-scmi.dtsi"

     /* include the generated options file */
     #include "fwk-dt-options.dtsi"

     / {
     	model = "Juno Development Platform (r0-r2)";
        	compatible = "arm,juno";


     };          

appears in `/product/juno/scp_ramfw` with the .dtsi files appearing in the `/dts/include` and `/dts/include/juno` directories. 

#### Product `CmakeLists.txt`

These `SCP_DT_CONFIG_..` list variables are set in the product `CMakesLists.txt` file. These list the modules that will use the DT build flow, if selected for the product. 

    # select device tree configured modules
    # These use the .dts configured variants
    # Order is not important for this list
    list(APPEND SCP_DT_CONFIG_MODULES_DYN "clock")
    list(APPEND SCP_DT_CONFIG_MODULES_STD "pl011")
    list(APPEND SCP_DT_CONFIG_MODULES_STD "mock-clock")

The product `CMakelists.txt` file can then be adjusted to remove all the config_<module>.c files for DT selected modules, and add any callback handlers for modules selected with SCP_DT_CONFIG_MODULES_DYN

Additional headers that must be processed in the `dt_gen_enum_defines.py` can also be added here. 

If there are build time options that necessitate additional optional .dtsi include files then the variables SCP_MODULE_DT_OPTIONS_DEFINE_REQ and SCP_MODULE_DT_OPTIONS_FILE_REQ can be set to list defines and the appropriate file to include.

e.g from juno/scp_romfw/CMakelists.txt:

    # add in juno specific headers for device tree bindings processing,
    # that are not include by other juno module builds
    list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_ppu_idx.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/juno_sds.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/software_mmap.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/scp_mmap.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/system_clock.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/system_mmap.h"
    )
    
    # create a list of optional .dtsi includes that will be added to the final dt compile
    # These are dependent on compile time defines which are generated later,
    # so add to define list and file list for later processing
    list(APPEND SCP_MODULE_DT_OPTIONS_DEFINE_REQ "BUILD_MODE_DEBUG")
    list(APPEND SCP_MODULE_DT_OPTIONS_FILE_REQ "juno-scp-romfw-debug-opts.dtsi")

When the build sets `BUILD_MODE_DEBUG`, the file `juno-scp-romfw-debug-opts.dtsi` will be added as an include in the `fwk-dt-options.dtsi` file.

The variables set must also be exported to the parent scope:

    # ensure include lists pushed to parent scope
    set(SCP_DT_BIND_H_GEN_FROM_INCL ${SCP_DT_BIND_H_GEN_FROM_INCL} PARENT_SCOPE)
    set(SCP_MODULE_DT_OPTIONS_DEFINE_REQ ${SCP_MODULE_DT_OPTIONS_DEFINE_REQ} PARENT_SCOPE)
    set(SCP_MODULE_DT_OPTIONS_FILE_REQ ${SCP_MODULE_DT_OPTIONS_FILE_REQ} PARENT_SCOPE)


### Using Devicetree to configure a module.

#### The config_dt_common `CMakeLists.txt`

There are a number of steps to creating the configuration files for a module to use devicetree.

1. Create appropriate .yaml bindings - or import from another source.
2. Create a config_dt_<module>.c file that can extract the information from the devicetree macros.
3. Adjust the module/config_dt_config CMakelists to build this file if the module is part of the SCP_DT_CONFIG_MODULES_ALL list, and add any headers that may be useful to be processed by the `dt_gen_enum_defines.py` script to the `SCP_DT_BIND_H_GEN_FROM_INCL` list.

e.g. The /module/config_dt_common/CMakelists.txt adds the following in the loop that checks for modules selected for DT configuration:

        # check module sds
        if("${SCP_MODULE}" STREQUAL "sds")
             set(dt_module_found TRUE)
             target_sources(${SCP_MODULE_TARGET}
                            PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/config_dt_sds.c")

             # add header file to list used to generate defines for dt include 
             list(APPEND SCP_DT_BIND_H_GEN_FROM_INCL "${CMAKE_CURRENT_SOURCE_DIR}/../sds/include/mod_sds.h")
       
        endif()
        # end sds

This adds the config_dt_sds.c file to the list of sources build with the module, and sets up headers to process for defines and enums to be used by the .dts / .dtsi source files. 

By convention the config source file is `config_dt_<module>.c`

For the common framework modules, all the DT configuration source files are in `module/config_dt_common`,
along with the `CMakelists.txt` file that processes these files, adding them to the module builds.

For product specific modules a similar `product/<prod_name>/module/config_dt_common` directory exists with source and `CMakelists.txt` for these modules.

The module can now be used in the DT flow by selecting it in the product `CMakeLists.txt` file, and adding nodes in the DT source.

These `config_dt_common` directories are added by the `module\CMakelists.txt` file as part of the process that builds the modules. 

#### The module `config_dt_<module>.c` file.

The module config_dt file **must** contain the `config_dt_<module>` variable that will be declared in the `fwk_module_list.c` file.

This will also contain the configuration generated from the DT macros.

For example - the clock module is configured as an array of `fwk_element` objects, with data structure `mod_clock_dev_config`.

In the juno `config_clock.c` which is in the product specific source directory, one table entry is as follows:

    static struct fwk_element clock_dev_desc_table[] = {
    ...
    
    [JUNO_CLOCK_IDX_HDLCDREFCLK] = {
        .name = "HDLCDREFCLK",
        .data = &((struct mod_clock_dev_config) {
            .driver_id = FWK_ID_ELEMENT_INIT(FWK_MODULE_IDX_JUNO_CDCEL937,
                JUNO_CLOCK_CDCEL937_IDX_HDLCDREFCLK),
            .api_id = FWK_ID_API_INIT(FWK_MODULE_IDX_JUNO_CDCEL937,
                                      MOD_JUNO_CDCEL937_API_IDX_CLOCK_DRIVER),
            .pd_source_id = FWK_ID_NONE_INIT,
        }),
    },

    ...
    };


The same configuration information will appear in the device tree as:

    scp-clk-dev@1 {
        compatible = "scp,clk-dev";
        name = "HDLCDREFCLK";
        scp-ids = <&elem_id FWK_MODULE_IDX_JUNO_CDCEL937 JUNO_CLOCK_CDCEL937_IDX_HDLCDREFCLK>,
                  <&elem_id FWK_MODULE_IDX_JUNO_CDCEL937 MOD_JUNO_CDCEL937_API_IDX_CLOCK_DRIVER>;
    };

Multiple instances can be added to the device tree to create the clock table.

Now as a common module the `config_dt_clock.c` file appears in the \module\clock\src directory, where it can be used by multiple products, not just a single product for a single variant.

This generates the same table using the DT macro API, which extracts values from the table for each instance appearing in the device tree.

First we declare a compatible which matches the one in the device tree `"scp,clk-dev"`:

    #define CLK_DEV_COMPAT scp_clk_dev

Next we use the macro API to build the table. DT_ macros are taken from the imported Zephyr headers, FWK_DT_ macros are extensions written for the SCP framework.

We can create the table object which uses a DT_ macro generator: 

    /* data table */
    static struct fwk_element clock_element_table[] = {
        // Macro for array elements 
        DT_FOREACH_OKAY_INST_scp_clk_dev(CFG_CLK_DEV_ELEM_INIT)
        // last null element
        [DT_N_INST_scp_clk_dev_NUM_OKAY] = { 0 },
    };


This uses the DT_FOREACH macro to create n elements in the table, where n is the number of instances of the compatible in the device tree.

DT_FOREACH executes n instances of:

    #define CFG_CLK_DEV_ELEM_INIT(n) \
        [n] = { \
        .name = FWK_DT_INST_PROP(CLK_DEV_COMPAT, n, name), \
        .data = &((struct mod_clock_dev_config) { \
            CFG_CLK_DEV_ELEM_DATA_PH(n) \
            }), \
        },

which in turn executes n instances of:

    /*
     * driver id and api id are required, pd source id is optional
     * DT array order must match this.
     */
    #define CFG_CLK_DEV_ELEM_DATA_PH(n) \
        .driver_id = FWK_DT_PH_IDX_SCP_ID_ELEM(CLK_DEV_COMPAT, n, 0), \
        .api_id = FWK_DT_PH_IDX_SCP_ID_ELEM(CLK_DEV_COMPAT, n, 1), \
        .pd_source_id = FWK_DT_PH_OPT_IDX_SCP_ID_ELEM(CLK_DEV_COMPAT, n, 2),


finally we declare the config structure that references out generated table:

    /* config structure */
    struct fwk_module_config config_dt_clock = {
        #ifdef FWK_MODULE_GEN_DYNAMIC_CLOCK
        .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dyn_get_element_table_clock),
        #else
        .elements = FWK_MODULE_STATIC_ELEMENTS_PTR(clock_element_table),
        #endif
    };


See macro documentation and source for further examples.

### Summary of files and variables used in Devicetree support

#### Used in C source code:

- `fwk_devicetree.h` : C header containing the macros to interpret the generated device tree information.
- `fwk_dt_config.h` : C header containing the macros generated from the device tree source.
- `fwk_dt_config_dyn.h` : C header containing callbacks for dynamic configured modules.
- `fwk_dt_config_common.h` : C header containing additional macros to interpret SCP specific DT elements. Also includes above three headers so can be used as only header in a C source impementing DT support.


#### Used in .dts / .dtsi files

- `fwk_dt_gen_bindings.h` : Generated header file containing #define values for use in device tree.
- `fwk_dt_mod_idx_gen.h` : Header containing #define values for the module index for all modules in the product.
- `fwk_dt_bindings.h` : Header file containing any additional fixed #define values for device tree support. Also includes the above two files and is included in `base.dtsi`, which must be the first include in any product .dts file.
- `fwk-dt-options.dtsi` : file generated with additional include .dtsi files according to build options for the product.

#### Tools and scripts

- `dts.cmake` : main DT processing CMake file.
- `dt_gen_config.py` : Device tree compiler and C macro generation script. Based on similar script from Zephyr. Uses the devicetree python module.
- `dt_check_py_modules.py` : Checks that the correct python pre-requisites are present in the development environment.
- `dt_gen_enum_defines.py` : generates `fwk_dt_gen_bindings.h` by processin input .h files, extracting #defines as declared, and enum values as #define values. 
 
#### CMake Variables

DT build flow control.

- `SCP_FIRMWARE_DTS_SOURCE` : Path and filename of the device tree source for the current product. This being set is used to trigger all other DT build flow operations.
- `SCP_USE_DT_BUILD` : Input variable to main CMakeLists.txt file. If set will cause `SCP_FIRMWARE_DTS_SOURCE` to be set in products that use DT build flow,. If omitted then standard non DT build will be used.

DT build module selection.

- `SCP_DT_CONFIG_MODULES_STD` : List of modules using the DT build flow, with standard fixed definitions.
- `SCP_DT_CONFIG_MODULES_DYN` : List of modules using the DT build flow with dynamic access and callbacks for product overrides at runtime.
- `SCP_DT_CONFIG_MODULES_ALL` : Combination of above two lists, passed to build all DT capable modules which will use DT build if on the list.
- `SCP_DT_PRODUCT_MODULE_DIR` : Path to the products module directory. This allows the main module CMakelists.txt process the product modules that use DT configuration.

DT bindings header file value generation (`fwk_dt_gen_bindings.h`).

- `SCP_DT_BIND_H_GEN_DEFS` : List of -D<define value> used when processing .h files to extract #define and enum values for  `fwk_dt_gen_bindings.h`. Generally same as those used when building the C code for the firmware.
- `SCP_DT_BIND_H_GEN_FROM_INCL` : List of .h files to process when generating  `fwk_dt_gen_bindings.h`
- `DTS_GEN_BINDINGS` : this is #define input to the `dt_check_enum_defines.py`. This define may be used to filter out sections of header files if these produce incompatible outputs for the DT compilation process.

DT optional .dtsi file generation ( `fwk-dt-options.dtsi`).

- `SCP_MODULE_DT_OPTIONS_DTSI` : List of optional .dtsi files that are added to `fwk-dt-options.dtsi` during CMake generation process.
- `SCP_MODULE_DT_OPTIONS_DEFINE_REQ` and `SCP_MODULE_DT_OPTIONS_FILE_REQ` : Pair of lists containing a build time define and an optional .dtsi file to use if the define is present. e.g. if the DEFINE list contains `BUILD_HAS_SCMI_NOTIFICATIONS`, and the FILE list has `juno-scp-notification-opts.dtsi` at the same index, then this file will be added to `fwk-dt-options.dtsi`.

### Additional information.

The  `fwk_devicetree.h` and `fwk_dt_config_common.h` have doxygen comments describing the usage of the macros.

More information on the device tree bindings and usage can be seen in the documentation for the Zephyr project.