#  Build a Hello World example {#GETTING_STARTED_BUILD}

[TOC]

## Using CCS projects

- **When using CCS projects to build**, make sure you have imported and built the CCS project of interest (see \ref CCS_PROJECTS_PAGE)

- Quick steps to import the "hello world example" CCS project are listed below
  - In CCS, goto "Toolbar > View > Project Explorer"
  - In the "Project Explorer" window, right click and select "Import > CCS Projects"
  - Click "Browse" and goto the folder `${SDK_INSTALL_DIR}/examples/hello_world`
\cond !SOC_AM62X && !SOC_AM62AX && !SOC_AM62PX && !SOC_AM62DX
  - Select the project `hello_world_{board}_r5fss0-0_freertos_ti-arm-clang`
\endcond
\cond SOC_AM62X
  - For MCU M4F
    - Select the project `hello_world_{board}_m4fss0-0_freertos_ti-arm-clang`

  - For WKUP R5F (DM R5)
    - Select the project `hello_world_{board}_r5fss0-0_freertos_ti-arm-clang`

  - For A53
    - Select the project `hello_world_{board}_a53ss0-0_freertos_gcc-aarch64`
\endcond
\cond SOC_AM62AX || SOC_AM62DX
  - For MCU R5F
    - Select the project `hello_world_{board}__mcu-r5fss0-0_freertos_ti-arm-clang`

  - For WKUP R5F (DM R5)
      - Select the project `hello_world_{board}_r5fss0-0_freertos_ti-arm-clang`

  - For A53
    - Select the project `hello_world_{board}_a53ss0-0_freertos_gcc-aarch64`

  - For C75
      - Select the project `hello_world_{board}_c75ss0-0_freertos_ti-c7000`

\endcond

\cond SOC_AM62PX
  - For MCU R5F
    - Select the project `hello_world_{board}_mcu-r5fss0-0_freertos_ti-arm-clang`

  - For WKUP R5F (DM R5)
      - Select the project `hello_world_{board}_wkup-r5fss0-0_freertos_ti-arm-clang`

\endcond

  - The project will be imported into the "Project Explorer"
  - Right click on the project title and click "Build Project"
  - This will build the project, now you can load and run it (see \ref CCS_LAUNCH_PAGE)

## Using makefiles

- **When using makefiles to build**, make sure you have built the example of interest using makefiles (see \ref MAKEFILE_BUILD_PAGE)

- Given below is the command to build the "hello world example". Note, use `gmake` in Windows

\cond !SOC_AM62X && !SOC_AM62AX && !SOC_AM62PX && !SOC_AM62DX
        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/r5fss0-0_freertos/ti-arm-clang
\endcond
\cond SOC_AM62X
  - For MCU M4F

        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/m4fss0-0_freertos/ti-arm-clang

  - For WKUP R5F (DM R5)

        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/r5fss0-0_freertos/ti-arm-clang

  - For A53

        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/a53ss0-0_freertos/gcc-aarch64
\endcond
\cond SOC_AM62AX || SOC_AM62DX
  - For MCU R5F

        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/mcu-r5fss0-0_freertos/ti-arm-clang

  - For WKUP R5F (DM R5)

        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/r5fss0-0_freertos/ti-arm-clang

  - For A53

        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/a53ss0-0_freertos/gcc-aarch64

  - For C75

        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/c75ss0-0_freertos/ti-c7000
\endcond
\cond SOC_AM62PX
  - For MCU R5F

        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/mcu-r5fss0-0_freertos/ti-arm-clang

  - For WKUP R5F (DM R5)

        cd ${SDK_INSTALL_PATH}
        make -s -C examples/hello_world/{board}/wkup-r5fss0-0_freertos/ti-arm-clang
\endcond
- After the executable is built successfully, now you can load and run it (see \ref CCS_LAUNCH_PAGE)

## Rebuilding libraries

- When a CCS example project is built or a single example is built with makefiles, as described above, the libraries are not re-built.
- If you have modified any source or header file with a library, you need to rebuild the libraries using makefiles.
- By default the SDK when installed has all libraries pre-built, however if you want to rebuild the libraries, do below

        cd ${SDK_INSTALL_PATH}
        make -s libs PROFILE=debug      # for debug mode libraries
        # OR
        make -s libs PROFILE=release    # for release mode libraries

- See also \ref MAKEFILE_BUILD_PAGE for more library build options and makefile tips.