# UART Echo {#EXAMPLES_DRIVERS_UART_ECHO}

[TOC]

# Introduction

This example demonstrates the UART RX and TX operation in blocking,
interrupt mode of operation.
This example receives 8 characters and echos back the same.
The application ends when the user types 8 characters.

\cond SOC_AM62X || SOC_AM62AX || SOC_AM62DX
To modify the example to use main domain UART, refer \ref MAIN_DOMAIN_PERIPHERAL_FROM_MCU
\endcond

# Supported Combinations {#EXAMPLES_DRIVERS_UART_ECHO_COMBOS}

\cond SOC_AM64X
\attention A53 NORTOS, A53 FREERTOS and A53 FREERTOS SMP support is experimental and is NOT supported by TI. \n
\endcond

\cond SOC_AM64X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 freertos
 ^              | r5fss0-0 nortos
 ^              | m4fss0-0 nortos
 ^              | a53ss0-0 nortos
 ^              | a53ss0-0 freertos
 ^              | a53ss0-0 freertos-smp
 Toolchain      | ti-arm-clang
 ^              | arm.gnu.aarch64-none
 Board          | @VAR_BOARD_NAME_LOWER, @VAR_SK_BOARD_NAME_LOWER
 Example folder | examples/drivers/uart/uart_echo

\endcond

\cond SOC_AM243X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 freertos
 ^              | r5fss0-0 nortos
 ^              | m4fss0-0 nortos(As am243x-lp has no MCU UART used for console application, M4 core support excluded for am243x-lp)
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/drivers/uart/uart_echo

\endcond

\cond SOC_AM263X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 freertos
 ^              | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/drivers/uart/uart_echo

\endcond

\cond SOC_AM273X || SOC_AWR294X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0 freertos
 ^              | r5fss0-0 nortos
 ^              | c66ss0 nortos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/uart/uart_echo

\endcond

\cond SOC_AM62X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | m4fss0-0 nortos
 ^              | r5fss0-0 freertos
 ^              | a53ss0-0 freertos
 Toolchain      | ti-arm-clang
 ^              | arm.gnu.aarch64-none
 Board          | @VAR_BOARD_NAME_LOWER, @VAR_SIP_SK_BOARD_NAME_LOWER
 Example folder | examples/drivers/uart/uart_echo

\endcond

\cond SOC_AM62AX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | mcu-r5fss0-0 nortos
 ^              | a53ss0-0 nortos
 Toolchain      | ti-arm-clang
 ^              | arm.gnu.aarch64-none
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/uart/uart_echo

\endcond

\cond SOC_AM62DX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | a53ss0-0 nortos
 ^              | a53ss0-0 freertos
 ^              | c75ss0-0 freertos
 ^              | mcu-r5fss0-0 nortos
 ^              | mcu-r5fss0-0 freertos
 ^              | r5fss0-0 freertos
 ^              | r5fss0-0 nortos
 Toolchain      | ti-arm-clang
 ^              | arm.gnu.aarch64-none
 ^              | ti-c7000
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/uart/uart_echo

\endcond

\cond SOC_AM62PX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | mcu-r5fss0-0 nortos
 ^              | wkup-r5fss0-0 freertos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/drivers/uart/uart_echo

\endcond
# Steps to Run the Example

- **When using CCS projects to build**, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- **When using makefiles to build**, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE)
- Launch a CCS debug session and run the executable, see \ref CCS_LAUNCH_PAGE
\cond SOC_AM62X || SOC_AM62PX
\attention As the wake-up R5 is the device manager, it needs to be started by the SBL. So it can not be loaded through CCS. It should be flashed and booted through SBL.

- Refer \ref GETTING_STARTED_FLASH for flashing the application.
\endcond

- Please select the UART port used for console as mentioned in \ref CCS_UART_TERMINAL

# See Also

\ref DRIVERS_UART_PAGE

# Sample Output

Shown below is a sample output when the application is run,
Please note that application prints in both CCS and UART console.
In UART console you need to enter 8 characters.

CCS Console:
\code
[MAIN_Cortex_R5_0_0] [UART] Echo example started ...
All tests have passed!!
\endcode

UART Console:
\code
This is uart echo test blocking mode
Receives 8 characters then echo's back. Please input..
12345678
All tests have passed!!
\endcode
