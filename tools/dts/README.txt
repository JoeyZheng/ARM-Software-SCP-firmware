Device tree python scripts and library
--------------------------------------

1) dt_gen_config.py

This is a script based largely on the on the equivalent script in the Zephyr
project (zephyr/scripts/dts/gen_defines.py).

This generated C #defines from devicetree files to provide a complile time
extraction of device tree information.

The main device tree library is located in scp/contrib/python-devicetree
This is copied as is from zephyr/scripts/dts/python-devicetree.

The README.txt in zephyr/scripts/dts indicates that there is a plan to make
the library standalone an installable from PyPi

Once this occurs then it can be installed on a build machine / copied from there
rather than be part of the SCP project repository.

2) dt_gen_enum_defines.py

This script takes as its input a set of .h header files. An output file is created
with the processed enums and #defines from the input files.

Any enum in these files is converted into a set of #define values for use in the
device tree source files.

e.g.
enum {
     MY_THING,
     MY_OTHER_THING,
     SOMETHING_ELSE
     }

will appear in the output file as

#define MY_THING 0
#define MY_OTHER_THING 1 
#define SOMETHING_ELSE 2

The #defines in these files are passed through

This allows this output file to be include in device tree source files, as enums
cannot be processed by the DT processing script.

Thus enum values in any of the framework headers can be processed and the equivalent
#define values used in the DT source files. This allows the same symbols to be used in
both C and DT source code.

test_enum_val.h can be used to test this script.

3) dt_check_py_modules.py

Ensures that the python modules 'devicetree', 'pcpp' and 'cxxheaderparser' are available to the
python command line.

'devicetree' is the library mentioned above, located in <scp>/contrib/python-devicetree, used by
dt_gen_config.py.

'pcpp' and 'cxxheaderparser' are open source libraries which must be loaded into the build machine from there respective public repositories - or via pip etc if possible.

These are used by the dt_gen_enum_defines script

4) scp-dt-build-juno-fw.bash

This is a helper script to build the juno firmware using the DT build flow.
Feeds the correct parameters into the Makefile.cmake file. 

