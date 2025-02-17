#  Flash an example application {#GETTING_STARTED_FLASH}

[TOC]

\attention Flashing an application will overwrite the SOC init application that was flashed earlier.
           So if you want to load and run from CCS again, you will need to do the SOC init steps again.
           See \ref EVM_SOC_INIT for more details.



\attention As the wake-up R5 is the device manager, it needs to be started by the SBL. So it can not be loaded through CCS. It should be flashed and booted through SBL.

\note See also \ref TOOLS_FLASH for more details on the flashing tools.

## Introduction

In this step we will flash the application that we have build and run using CCS to the EVM flash.
We can then boot this application without being connected to CCS via JTAG.

## Getting ready to flash the application

- A quick recap of steps done so far that are needed for the flashing to work
  - Make sure the UART port used for terminal is identified as mentioned in \ref CCS_UART_TERMINAL
  - Make sure python3 is installed as mentioned in \ref INSTALL_PYTHON3
  - Make sure you have the EVM power cable and UART cable connected as shown in \ref EVM_CABLES

- Build the hello world application as mentioned in \ref GETTING_STARTED_BUILD

- As part of the build process in the final step a file with extension `.appimage` is generated. This is the file
  we need to flash.

  - When building with makefiles and single-core projects, this file can be found here (shown for hello world example),

    - For M4F

          ${SDK_INSTALL_PATH}/examples/hello_world/{board}/m4fss0-0_freertos/ti-arm-clang/hello_world.release.appimage

    - For R5F

          ${SDK_INSTALL_PATH}/examples/hello_world/{board}/r5fss0-0_freertos/ti-arm-clang/hello_world.release.appimage

    - For A53

          ${SDK_INSTALL_PATH}/examples/hello_world/{board}/a53ss0-0_freertos/gcc-aarch64/hello_world.release.appimage

  - When building with CCS projects, this file can be found here (shown for hello world example),

    - For M4F

          ${CCS_WORKSPACE_PATH}/hello_world_{board}_m4fss0-0_freertos_ti-arm-clang/Release/hello_world_{board}_m4fss0-0_freertos_ti-arm-clang.appimage

    - For R5F

          ${CCS_WORKSPACE_PATH}/hello_world_{board}_r5fss0-0_freertos_ti-arm-clang/Release/hello_world_{board}_r5fss0-0_freertos_ti-arm-clang.appimage

    - For A53

          ${CCS_WORKSPACE_PATH}/hello_world_{board}_a53ss0-0_freertos_ti-arm-clang/Release/hello_world_{board}_a53ss0-0_freertos_gcc-aarch64.appimage

  - **NOTE**: The folder name and file name in path can have "release", "Release" or "debug", "Debug" based on the profile that the application is built with.

- Next, we need to list the files to flash in a flash configuration file. A default configuration file can be found at below path.
  You can edit this file directly or take a copy and edit this file.

    - For am62x-sk (Flashing Linux on A53)

          ${SDK_INSTALL_PATH}/tools/boot/sbl_prebuilt/am62x-sk/default_sbl_ospi_linux_hs_fs.cfg


    - For am62x-sk-lp (Flashing Linux on A53)

          ${SDK_INSTALL_PATH}/tools/boot/sbl_prebuilt/am62x-sk-lp/default_sbl_ospi_nand_linux_hs_fs.cfg

    - For am62x-sip-sk (Flashing Linux on A53)

          ${SDK_INSTALL_PATH}/tools/boot/sbl_prebuilt/am62x-sip-sk/default_sbl_ospi_linux_hs_fs.cfg

    - For am62x-sk (Flashing FreeRTOS on A53)

          ${SDK_INSTALL_PATH}/tools/boot/sbl_prebuilt/am62x-sk/default_sbl_ospi_hs_fs.cfg

    - For am62x-sk-lp (Flashing FreeRTOS on A53)

          ${SDK_INSTALL_PATH}/tools/boot/sbl_prebuilt/am62x-sk-lp/default_sbl_ospi_nand_hs_fs.cfg

    - For am62x-sip-sk (Flashing FreeRTOS on A53)

          ${SDK_INSTALL_PATH}/tools/boot/sbl_prebuilt/am62x-sip-sk/default_sbl_ospi_hs_fs.cfg

\note For HS-SE device, use default_sbl_ospi_linux_hs.cfg as the cfg file.
\note For HS-FS device, use default_sbl_ospi_linux_hs_fs.cfg as the cfg file.
\note For HS-SE device, use default_sbl_ospi_hs.cfg as the cfg file.
\note For HS-FS device, use default_sbl_ospi_hs_fs.cfg as the cfg file.

- For Linux application Edit below line in the config file to point to your application `.appimage` file.
  Give the absolute path to the `.appimage` file or path relative to `${SDK_INSTALL_PATH}/tools/boot`. **Make sure to use forward slash `/` in the filename path**.

    - For M4F

          --file=../../examples/drivers/ipc/ipc_rpmsg_echo_linux/{board}/m4fss0-0_freertos/ti-arm-clang/ipc_rpmsg_echo_linux.release.appimage --operation=flash --flash-offset=0x100000

    - For R5F

          --file=../../examples/drivers/ipc/ipc_rpmsg_echo_linux/{board}/r5fss0-0_freertos/ti-arm-clang/ipc_rpmsg_echo_linux.release.appimage --operation=flash --flash-offset=0xA00000

- For FreeRTOS application Edit below line in the config file to point to your application `.appimage` file.
  Give the absolute path to the `.appimage` file or path relative to `${SDK_INSTALL_PATH}/tools/boot`. **Make sure to use forward slash `/` in the filename path**.

    - For M4F

          --file=../../examples/drivers/ipc/ipc_rpmsg_echo/{board}/m4fss0-0_freertos/ti-arm-clang/ipc_rpmsg_echo.release.appimage --operation=flash --flash-offset=0x100000

    - For R5F

          --file=../../examples/drivers/ipc/ipc_rpmsg_echo/{board}/r5fss0-0_freertos/ti-arm-clang/ipc_rpmsg_echo.release.appimage --operation=flash --flash-offset=0xA00000

    - For A53

          --file=../../examples/drivers/ipc/ipc_rpmsg_echo_/{board}/a53ss0-0_freertos/gcc-aarch64/ipc_rpmsg_echo.release.appimage --operation=flash --flash-offset=0xC00000

- This file will additionally also list the flashing application that is run on the EVM and a OSPI flash bootloader that also
  needs to be flashed. You can keep this unchanged if you have not modified these applications.

- Save and close the config file.

## Building linux app image and HSM  app image

The linux and HSM app images are to be generated to flash along with your application for MCU M4.

### LinuxAppImage
\note For HS-SE device, use DEVICE_TYPE=HS option in the makefile.

 - Ensure the AM62X Processor SDK Linux path is correct in the `${SDK_INSTALL_PATH}/tools/boot/linuxAppimageGen/board/{board_name}/config.mak` file.

 - Go to `${SDK_INSTALL_PATH}/tools/boot/linuxAppimageGen` on terminal
 - Run the following command to build the linux app image.
    - For @VAR_BOARD_NAME
    \code
    make BOARD=am62x-sk all
    \endcode

    - For @VAR_SK_LP_BOARD_NAME
    \code
    make BOARD=am62x-sk-lp all
    \endcode

    - For @VAR_SIP_SK_BOARD_NAME
    \code
    make BOARD=am62x-sip-sk all
    \endcode


### HSMAppImage

\note For HS-SE device, use DEVICE_TYPE=HS option in the makefile.

 - Go to `${SDK_INSTALL_PATH}/tools/boot/HSMAppimageGen` on terminal
 - Run the following command to build the HSM app image.
    - For @VAR_BOARD_NAME
    \code
    make BOARD=am62x-sk all
    \endcode

    - For @VAR_SK_LP_BOARD_NAME
    \code
    make BOARD=am62x-sk-lp all
    \endcode

    - For @VAR_SIP_SK_BOARD_NAME
    \code
    make BOARD=am62x-sip-sk all
    \endcode

## Flashing the application
- Build all the binaries
    - For Linux
    \code
        make DEVICE=am62x -sj
    \endcode
    - For Windows
    \code
        gmake DEVICE=am62x -sj
    \endcode

- **POWER-OFF** the EVM

- Set boot mode to UART BOOTMODE as shown in below image

  \imageStyle{boot_pins_uart_boot_mode.png,width:30%}
  \image html boot_pins_uart_boot_mode.png "UART BOOT MODE"

- **POWER-ON** the EVM

- You should see character "C" getting printed on the UART terminal every 2-3 seconds as shown below

  \imageStyle{uart_rom_boot.png,width:80%}
  \image html uart_rom_boot.png "UART output in UART BOOT MODE"

- Close the UART terminal as shown below. This is important, else the UART script in next step wont be able to connect to the UART port.

  \imageStyle{ccs_uart_close.png,width:80%}
  \image html ccs_uart_close.png "Close UART terminal"

\note For am62x-sk, am62x-sip-sk HS-SE device, use default_sbl_ospi_linux_hs.cfg as the cfg file.
\note For am62x-sk, am62x-sip-sk HS-FS device, use default_sbl_ospi_linux_hs_fs.cfg as the cfg file.
\note For am62x-sk-lp HS-SE device, use default_sbl_ospi_nand_linux_hs.cfg as the cfg file.
\note For am62x-sk-lp HS-FS device, use default_sbl_ospi_nand_linux_hs_fs.cfg as the cfg file.
\note For am62x-sk, am62x-sip-sk HS-SE device, use default_sbl_ospi_hs.cfg as the cfg file.
\note For am62x-sk, am62x-sip-sk HS-FS device, use default_sbl_ospi_hs_fs.cfg as the cfg file.
\note For am62x-sk-lp HS-SE device, use default_sbl_ospi_nand_hs.cfg as the cfg file.
\note For am62x-sk-lp HS-FS device, use default_sbl_ospi_nand_hs_fs.cfg as the cfg file.

- Open a command prompt and run the below command to flash the SOC initialization binary to the EVM.
    - For @VAR_BOARD_NAME (Flashing Linux on A53)

            cd ${SDK_INSTALL_PATH}/tools/boot
            python uart_uniflash.py -p COM13 --cfg=sbl_prebuilt/@VAR_BOARD_NAME_LOWER/default_sbl_ospi_linux_hs_fs.cfg

    - For @VAR_SK_LP_BOARD_NAME (Flashing Linux on A53)

            cd ${SDK_INSTALL_PATH}/tools/boot
            python uart_uniflash.py -p COM13 --cfg=sbl_prebuilt/@VAR_SK_LP_BOARD_NAME_LOWER/default_sbl_ospi_nand_linux_hs_fs.cfg

    - For @VAR_SIP_SK_BOARD_NAME (Flashing Linux on A53)

            cd ${SDK_INSTALL_PATH}/tools/boot
            python uart_uniflash.py -p COM13 --cfg=sbl_prebuilt/@VAR_SIP_SK_BOARD_NAME_LOWER/default_sbl_ospi_linux_hs_fs.cfg

    - For @VAR_BOARD_NAME (Flashing FreeRTOS on A53)

            cd ${SDK_INSTALL_PATH}/tools/boot
            python uart_uniflash.py -p COM13 --cfg=sbl_prebuilt/@VAR_BOARD_NAME_LOWER/default_sbl_ospi_hs_fs.cfg

    - For @VAR_SK_LP_BOARD_NAME (Flashing FreeRTOS on A53)

            cd ${SDK_INSTALL_PATH}/tools/boot
            python uart_uniflash.py -p COM13 --cfg=sbl_prebuilt/@VAR_SK_LP_BOARD_NAME_LOWER/default_sbl_ospi_nand_hs_fs.cfg

    - For @VAR_SIP_SK_BOARD_NAME (Flashing FreeRTOS on A53)

            cd ${SDK_INSTALL_PATH}/tools/boot
            python uart_uniflash.py -p COM13 --cfg=sbl_prebuilt/@VAR_SIP_SK_BOARD_NAME_LOWER/default_sbl_ospi_hs_fs.cfg

    - Here COM13 is the port name of the identified UART port in Windows.
    - On Linux,
        - The name for UART port is typically something like `/dev/ttyUSB0`
        - On some Linux systems, one needs to use `python3` to invoke python3.x, just `python` command may invoke python 2.x which will not work with the flashing script.

- When the flashing is in progress you will see something like below

  \imageStyle{flash_soc_init_in_progress.png,width:80%}
  \image html flash_soc_init_in_progress.png "Flash in progress"

- After all the applications and linuxappimage flashing is done, you will see something like below

        Parsing config file ...
        Parsing config file ... SUCCESS. Found 9 command(s) !!!

        Executing command 1 of 9 ...
        Found flash writer ... sending sbl_prebuilt/am62x-sk/sbl_uart_uniflash_stage1.release.tiimage
        Sent flashwriter sbl_prebuilt/am62x-sk/sbl_uart_uniflash_stage1.release.tiimage of size 242313 bytes in 24.98s.

        Executing command 2 of 9 ...
        Command arguments : --file=../../examples/drivers/boot/sbl_uart_uniflash_multistage/sbl_uart_uniflash_stage2/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_uart_uniflash_stage2.release.appimage --operation=flash --flash-offset=0x0
        Sent ../../examples/drivers/boot/sbl_uart_uniflash_multistage/sbl_uart_uniflash_stage2/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_uart_uniflash_stage2.release.appimage of size 116008 bytes in 12.46s.
        [STATUS] SUCCESS !!!

        Executing command 3 of 9 ...
        Command arguments : --operation=flash-phy-tuning-data
        Sent flash phy tuning data in 3.87s.
        [STATUS] SUCCESS !!!

        Executing command 4 of 9 ...
        Command arguments : --file=../../examples/drivers/boot/sbl_ospi_linux_multistage/sbl_ospi_linux_stage1/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_ospi_linux_stage1.release.tiimage --operation=flash --flash-offset=0x0
        Sent ../../examples/drivers/boot/sbl_ospi_linux_multistage/sbl_ospi_linux_stage1/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_ospi_linux_stage1.release.tiimage of size 248841 bytes in 26.43s.
        [STATUS] SUCCESS !!!

        Executing command 5 of 9 ...
        Command arguments : --file=../../examples/drivers/boot/sbl_ospi_linux_multistage/sbl_ospi_linux_stage2/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_ospi_linux_stage2.release.appimage --operation=flash --flash-offset=0x80000
        Sent ../../examples/drivers/boot/sbl_ospi_linux_multistage/sbl_ospi_linux_stage2/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_ospi_linux_stage2.release.appimage of size 101764 bytes in 12.6s.
        [STATUS] SUCCESS !!!

        Executing command 6 of 9 ...
        Command arguments : --file=../../examples/drivers/ipc/ipc_rpmsg_echo_linux/am62x-sk/m4fss0-0_freertos/ti-arm-clang/ipc_rpmsg_echo_linux.release.appimage --operation=flash --flash-offset=0x100000
        Sent ../../examples/drivers/ipc/ipc_rpmsg_echo_linux/am62x-sk/m4fss0-0_freertos/ti-arm-clang/ipc_rpmsg_echo_linux.release.appimage of size 50316 bytes in 7.8s.
        [STATUS] SUCCESS !!!

        Executing command 7 of 9 ...
        Command arguments : --file=../../tools/boot/HSMAppimageGen/board/am62x-sk/hsm.appimage --operation=flash --flash-offset=0x800000
        Sent ../../tools/boot/HSMAppimageGen/board/am62x-sk/hsm.appimage of size 124 bytes in 3.1s.
        [STATUS] SUCCESS !!!

        Executing command 8 of 9 ...
        Command arguments : ../../examples/drivers/ipc/ipc_rpmsg_echo_linux/am62x-sk/r5fss0-0_freertos/ti-arm-clang/ipc_rpmsg_echo_linux.release.appimage --operation=flash --flash-offset=0xA00000
        Sent ../../examples/drivers/ipc/ipc_rpmsg_echo_linux/am62x-sk/r5fss0-0_freertos/ti-arm-clang/ipc_rpmsg_echo_linux.release.appimage of size 138180 bytes in 15.96s.
        [STATUS] SUCCESS !!!

        Executing command 9 of 9 ...
        Command arguments : --file=../../tools/boot/linuxAppimageGen/board/am62x-sk/linux.appimage --operation=flash --flash-offset=0xC00000
        Sent ../../tools/boot/linuxAppimageGen/board/am62x-sk/linux.appimage of size 736528 bytes in 72.12s.
        [STATUS] SUCCESS !!!

        All commands from config file are executed !!!


-  After all the applications along with a53 freertos application flashing is done, you will see something like below

        Parsing config file ...
        Parsing config file ... SUCCESS. Found 9 command(s) !!!

        Executing command 1 of 9 ...
        Found flash writer ... sending sbl_prebuilt/am62x-sk/sbl_uart_uniflash_stage1.release.hs_fs.tiimage
        Sent flashwriter sbl_prebuilt/am62x-sk/sbl_uart_uniflash_stage1.release.hs_fs.tiimage of size 248888 bytes in 24.89s.

        Executing command 2 of 9 ...
        Command arguments : --file=../../examples/drivers/boot/sbl_uart_uniflash_multistage/sbl_uart_uniflash_stage2/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_uart_uniflash_stage2.release.appimage.hs_fs --operation=flash --flash-offset=0x0
        Sent ../../examples/drivers/boot/sbl_uart_uniflash_multistage/sbl_uart_uniflash_stage2/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_uart_uniflash_stage2.release.appimage.hs_fs of size 124375 bytes in 15.19s
        [STATUS] SUCCESS !!!

        Executing command 3 of 9 ...
        Command arguments : --operation=flash-phy-tuning-data
        Sent flash phy tuning data in 3.13s.
        [STATUS] SUCCESS !!!

        Executing command 4 of 9 ...
        Command arguments : --file=sbl_prebuilt/am62x-sk/sbl_ospi_stage1.release.hs_fs.tiimage --operation=flash --flash-offset=0x0
        Sent sbl_prebuilt/am62x-sk/sbl_ospi_stage1.release.hs_fs.tiimage of size 290104 bytes in 30.23s.
        [STATUS] SUCCESS !!!

        Executing command 5 of 9 ...
        Command arguments : --file=../../examples/drivers/boot/sbl_ospi_multistage/sbl_ospi_stage2/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_ospi_stage2.release.appimage.hs_fs --operation=flash --flash-offset=0x80000
        Sent ../../examples/drivers/boot/sbl_ospi_multistage/sbl_ospi_stage2/am62x-sk/r5fss0-0_nortos/ti-arm-clang/sbl_ospi_stage2.release.appimage.hs_fs of size 138495 bytes in 16.03s.
        [STATUS] SUCCESS !!!

        Executing command 6 of 9 ...
        Command arguments : --file=../../examples/hello_world/am62x-sk/m4fss0-0_freertos/ti-arm-clang/hello_world.release.appimage.hs_fs --operation=flash --flash-offset=0x100000
        Sent ../../examples/hello_world/am62x-sk/m4fss0-0_freertos/ti-arm-clang/hello_world.release.appimage.hs_fs of size 38575 bytes in 6.33s.
        [STATUS] SUCCESS !!!

        Executing command 7 of 9 ...
        Command arguments : --file=../../tools/boot/HSMAppimageGen/board/am62x-sk/hsm.appimage.hs_fs --operation=flash --flash-offset=0x800000
        Sent ../../tools/boot/HSMAppimageGen/board/am62x-sk/hsm.appimage.hs_fs of size 9646 bytes in 3.95s.
        [STATUS] SUCCESS !!!

        Executing command 8 of 9 ...
        Command arguments : --file=../../examples/hello_world/am62x-sk/r5fss0-0_freertos/ti-arm-clang/hello_world.release.appimage.hs_fs --operation=flash --flash-offset=0xA00000
        Sent ../../examples/hello_world/am62x-sk/r5fss0-0_freertos/ti-arm-clang/hello_world.release.appimage.hs_fs of size 129335 bytes in 16.05s.
        [STATUS] SUCCESS !!!

        Executing command 9 of 9 ...
        Command arguments : --file=../../examples/hello_world/am62x-sk/a53ss0-0_freertos/gcc-aarch64/hello_world.release.appimage.hs_fs --operation=flash --flash-offset=0xC00000
        Sent ../../examples/hello_world/am62x-sk/a53ss0-0_freertos/gcc-aarch64/hello_world.release.appimage.hs_fs of size 64431 bytes in 12.88s.
        [STATUS] SUCCESS !!!

        All commands from config file are executed !!!

- If flashing has failed, see \ref TOOLS_FLASH_ERROR_MESSAGES, and resolve the errors.

- If flashing is successful, do the next steps ...

## Running the flashed application

- **POWER-OFF** the EVM

- Switch the EVM boot mode to OSPI NOR mode incase of AM62X-SK and AM62X-SIP-SK(or) OSPI NAND incase of AM62X-SK-LP as shown below,
- For @VAR_BOARD_NAME and @VAR_SIP_SK_BOARD_NAME

    \imageStyle{boot_pins_ospi_mode.png,width:30%}
    \image html boot_pins_ospi_mode.png "OSPI NOR BOOT MODE (AM62X-SK/AM62X-SIP-SK)"

  - For @VAR_SK_LP_BOARD_NAME

    \imageStyle{boot_pins_ospi_nand_mode.png,width:30%}
    \image html boot_pins_ospi_nand_mode.png "OSPI NAND BOOT MODE  (AM62X-SK-LP)"

- Re-connect the UART terminal in CCS window as shown in \ref CCS_UART_TERMINAL

- **POWER-ON** the EVM

- You should see the application output in MCU UART terminal  as below

        Hello World!

- You should see the application output in WKUP UART terminal  as below

        Sciserver Testapp Built On: May  8 2024 10:16:43
        Sciserver Version: v2023.11.0.0REL.MCUSDK.MM.NN.PP.bb
        RM_PM_HAL Version: vMM.NN.PP
        Starting Sciserver..... PASSED
        Hello World!

- You should see the following SBL output and a53 application output on the main UART terminal as below.

        SYSFW Firmware Version 9.2.7--v09.02.07 (Kool Koala)
        SYSFW Firmware revision 0x9
        SYSFW ABI revision 3.1

        [BOOTLOADER_PROFILE] Boot Media       : FLASH
        [BOOTLOADER_PROFILE] Boot Media Clock : 166.667 MHz
        [BOOTLOADER_PROFILE] Boot Image Size  : 169 KB
        [BOOTLOADER_PROFILE] Cores present    :
        m4f0-0
        r5f0-0
        [BOOTLOADER PROFILE] System_init                      :       5506us
        [BOOTLOADER PROFILE] Board_init                       :          0us
        [BOOTLOADER PROFILE] Drivers_open                     :        297us
        [BOOTLOADER PROFILE] Board_driversOpen                :       6784us
        [BOOTLOADER PROFILE] Sciclient Get Version            :      10205us
        [BOOTLOADER PROFILE] App_waitForMcuPbist              :       4989us
        [BOOTLOADER PROFILE] App_waitForMcuLbist              :       7689us
        [BOOTLOADER PROFILE] App_loadImages                   :       3330us
        [BOOTLOADER PROFILE] App_loadSelfcoreImage            :       3944us
        [BOOTLOADER_PROFILE] SBL Total Time Taken             :      42748us

        Image loading done, switching to application ...
        Starting MCU-m4f and 2nd stage bootloader

        SYSFW Firmware Version 9.2.7--v09.02.07 (Kool Koala)
        SYSFW Firmware revision 0x9
        SYSFW ABI revision 3.1

        [BOOTLOADER_PROFILE] Boot Media       : FLASH
        [BOOTLOADER_PROFILE] Boot Media Clock : 166.667 MHz
        [BOOTLOADER_PROFILE] Boot Image Size  : 193 KB
        [BOOTLOADER_PROFILE] Cores present    :
        hsm-m4f0-0
        r5f0-0
        a530-0
        [BOOTLOADER PROFILE] System_init                      :       2823us
        [BOOTLOADER PROFILE] Board_init                       :          1us
        [BOOTLOADER PROFILE] Drivers_open                     :        368us
        [BOOTLOADER PROFILE] Board_driversOpen                :        106us
        [BOOTLOADER PROFILE] Sciclient Get Version            :      10253us
        [BOOTLOADER PROFILE] App_loadImages                   :       2684us
        [BOOTLOADER PROFILE] App_loadSelfcoreImage            :       3932us
        [BOOTLOADER PROFILE] App_loadA53Images                :       3781us
        [BOOTLOADER_PROFILE] SBL Total Time Taken             :      23952us

        Image loading done, switching to application ...
        Starting RTOS/Baremetal applications
        Hello World!


- Congratulations now the EVM is flashed with your application and you don't need CCS anymore to run the application.
