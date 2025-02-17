# FreeRTOS {#KERNEL_FREERTOS_PAGE}

[TOC]
\cond !SOC_AM62X && !SOC_AM62AX && !SOC_AM62DX
\note A53 will not be available on all SOCs. All references to A53 should be ignored on such SOCs.
\endcond

## Introduction

FreeRTOS is a market-leading real-time operating system (RTOS) for microcontrollers and small microprocessors. Distributed freely under the MIT open source license, FreeRTOS includes a kernel and a growing set of libraries suitable for use across all industry sectors. FreeRTOS is built with an emphasis on reliability and ease of use.

MCU+ SDK supports FreeRTOS on below CPUS
\cond !SOC_AM62AX && !SOC_AM62PX && !SOC_AM62DX
- ARM M4F
\endcond
\cond !SOC_AM62AX && !SOC_AM62DX
- ARM R5F
\endcond
\cond SOC_AM64X
- ARM A53 (single core and SMP on both cores)
\endcond
\cond SOC_AM62AX
- ARM DM_R5F
- ARM MCU_R5F
- ARM A53 (single core and SMP on quad cores)
- TI  C75
\endcond
\cond SOC_AM62DX
- ARM DM_R5F
- ARM MCU_R5F
- ARM A53 (single core)
- TI  C75
\endcond
\cond SOC_AM62X
- ARM A53 (single core)
\endcond

## Features Supported {#FREERTOS_SUPPORTED_FEATURES}

- FreeRTOS Kernel @VAR_FREERTOS_KERNEL_VERSION
  - Tasks, semaphores, mutex, queues, timers, list, heap, event groups
  - preemptive priority based scheduler
  - static and/or dynamic memory allocation mode
\cond !SOC_AM62X
- FreeRTOS SMP Kernel @VAR_FREERTOS_SMP_KERNEL_VERSION
  - Tasks, semaphores, mutex, queues, timers, list, heap, event groups
  - preemptive priority based scheduler for multiple cores
  - static and/or dynamic memory allocation mode
\endcond
- FreeRTOS+POSIX
  - Limited POSIX API wrappers on top of FreeRTOS APIs
  - Clock, message queue, pthread, pthread cond, pthread mutex, semaphore, timer
- In order to keep the device drivers agnostic of FreeRTOS or NORTOS, additionally below \ref KERNEL_DPL_PAGE APIs are implemented to call FreeRTOS APIs underneath,
  - Clock, task, semaphore, heap, cache, MPU, debug logs, HW interrupts, HW timers
- Floating point save/restore with tasks (make sure to call portTASK_USES_FLOATING_POINT() before using floating point operations )
\cond !SOC_AM62X && !SOC_AM62AX && !SOC_AM62AX
- R5F ISRs,
  - IRQ mode,
    - FPU save/restore is supported.
\cond SOC_AM263X
    - nested interrupts are supported.
\endcond
\endcond
\cond !SOC_AM62AX && !SOC_AM62AX
- M4F ISRs,
  - nested interrupts supported
\endcond
\cond SOC_AM62AX || SOC_AM62DX
- DM_R5F ISRs,
  - IRQ mode,
    - FPU save/restore is supported.
- MCU_R5 ISRs
  - IRQ mode,
    - nested interrupts supported
\endcond
\cond SOC_AM62X || SOC_AM62AX || SOC_AM62DX
- A53 ISRs,
  - IRQ mode,
    - nested interrupts supported
\endcond

## SysConfig Features

@VAR_SYSCFG_USAGE_NOTE

SysConfig can be used to configure below modules with FreeRTOS
- Clock module, to setup system tick timer including the tick duration
- Debug Log module, to select the console to use for logging as well as enable/disable logging zones
\cond !SOC_AM62AX && !SOC_AM62DX
- MPU ARMv7, to setup different MPU regions for R5F and M4F CPUs
\endcond
\cond SOC_AM62X || SOC_AM64X
- MMU ARMv8, to setup different MMU regions for A53 CPUs
\endcond
\cond SOC_AM62AX || SOC_AM62DX
- MPU ARMv7, to setup different MPU regions for DM_R5F and MCU_R5F CPUs
- MMU ARMv8, to setup different MMU regions for A53 CPUs and C75 core
\endcond
\cond !SOC_AM62AX && !SOC_AM62DX
- Address Translate module, to setup  address translation regions, needed for M4F
\endcond
\cond SOC_AM62AX || SOC_AM62DX
- Address Translate module, to setup  address translation regions, needed for DM_R5F and MCU_R5F
\endcond
- HW Timer module, to setup HW timer available on the SOC, including enabling timer interrupt and ISR registration

## Features Not Supported

- Co-routines, stream buffer are not enabled and are not compiled by default. Users can add these to the FreeRTOS config and makefile if they want to use these features.
- Tickless IDLE mode
- Task level memory protection wrapper
\cond !SOC_AM62X && !SOC_AM62AX && !SOC_AM62DX
- R5F ISRs,
\cond SOC_AM243X || SOC_AM64X || SOC_AWR294X || SOC_AM273X || SOC_AM62AX || SOC_AM62DX
  - IRQ mode,
    - nested interrupts is disabled, due to issues in some corner cases.
\endcond
  - FIQ mode,
    - nested interrupts not supported
    - FPU save/restore not supported.
\endcond
\cond !SOC_AM62AX && !SOC_AM62DX
- M4F ISRs,
  - FPU save/restore not supported.
\endcond
\cond SOC_AM62AX || SOC_AM62DX
- DM_R5F ISRs,
  - IRQ mode,
    - nested interrupts is disabled, due to issues in some corner cases.
  - FIQ mode,
    - nested interrupts not supported.
    - FPU save/restore not supported.
- MCU_R5F ISRs
  - FPU save/restore not supported.
\endcond
- A53 ISRs,
  - FPU save/restore not supported.


## Important files and directory structure

FreeRTOS source is distributed along with MCU+ SDK and given below are some important files and folders related to FreeRTOS.

<table>
<tr>
    <th>Folder/Files
    <th>Description
</tr>
<tr><td colspan="2" bgcolor=#F0F0F0> ${SDK_INSTALL_PATH}/source/kernel/</td></tr>
<tr>
    <td>dpl/
    <td>APIs to access FreeRTOS features in a OS agnostic way</td>
</tr>
<tr><td colspan="2" bgcolor=#F0F0F0> ${SDK_INSTALL_PATH}/source/kernel/freertos</td></tr>
<tr>
    <td>lib/
    <td>FreeRTOS library to link against. Linking to the library in this path enables the application to operate in FreeRTOS mode </td>
</tr>
<tr>
    <td>config/
    <td>FreeRTOS and FreeRTOS POSIX configuration header files for different CPUs within a SOC.
</tr>
<tr>
    <td>FreeRTOS-Kernel/
    <td>FreeRTOS Kernel source code. MCU+ SDK simply clones the code from FreeRTOS github and does not modify anything in this folder</td>
</tr>
\cond !SOC_AM62X
<tr>
    <td>FreeRTOS-Kernel-smp/
    <td>FreeRTOS Kernel source code for SMP. MCU+ SDK simply clones the code from FreeRTOS github and does not modify anything in this folder</td>
</tr>
\endcond
<tr>
    <td>FreeRTOS-POSIX/
    <td>FreeRTOS POSIX wrapper source code. MCU+ SDK simply clones the code from FreeRTOS POSIX github and does not modify anything in this folder</td>
</tr>
<tr>
    <td>portable/
    <td>MCU+ SDK FreeRTOS porting related files for different CPUs</td>
</tr>
<tr><td colspan="2" bgcolor=#F0F0F0> ${SDK_INSTALL_PATH}/source/kernel/freertos/dpl</td></tr>
<tr>
    <td>common/
    <td>FreeRTOS DPL APIs that are common across all CPUs
</tr>
<tr>
    <td>m4/
    <td>FreeRTOS DPL APIs that are specific to M4F CPUs
</tr>
<tr>
    <td>r5/
    <td>FreeRTOS APIs that are specific to R5F CPUs
</tr>

\cond SOC_AM62X || SOC_AM62AX || SOC_AM62DX
<tr>
    <td>a53/
    <td>FreeRTOS APIs that are specific to A53 CPUs
</tr>
\endcond
\cond SOC_AM62AX || SOC_AM62DX
<tr>
    <td>C75/
    <td>FreeRTOS APIs that are specific to C75 Core
</tr>
\endcond

</table>

In addition to above files, to enable features like HW interrupts, cache, MPU, the FreeRTOS library
in MCU+ SDK also shares some code with NORTOS, see to makefile in the folder `source\kernel\freertos` to see the exact list of files
that are included to build a freertos library.

## FreeRTOS usage guidelines

See \subpage KERNEL_FREERTOS_IMPORTANT_GUIDELINES_PAGE for FreeRTOS usage guidelines and comparison to SysBIOS.

## Additional references

Given below are some references to learn more about FreeRTOS.

<table>
<tr>
    <th>Document Description
    <th>Web link
</tr>
<tr>
    <td>Easy to read FreeRTOS book
    <td>https://www.freertos.org/Documentation/RTOS_book.html
</tr>
<tr>
    <td>FreeRTOS user docs
    <td>https://www.freertos.org/features.html
</tr>
<tr>
    <td>User API reference
    <td>https://www.freertos.org/a00106.html
</tr>
<tr>
    <td>FreeRTOS core kernel source code
    <td>https://github.com/FreeRTOS/FreeRTOS-Kernel
</tr>
\cond !SOC_AM62X && !SOC_AM62DX
<tr>
    <td>FreeRTOS core kernel source code for SMP
    <td>https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/smp
</tr>
\endcond
<tr>
    <td>FreeRTOS core kernel example source code
    <td>https://github.com/FreeRTOS/FreeRTOS/tree/master/FreeRTOS
</tr>
<tr>
    <td>Additional FreeRTOS.org maintained libraries (POSIX, TCP, Filesystem, …)
    <td>https://www.freertos.org/FreeRTOS-Labs/index.html \n
    https://www.freertos.org/FreeRTOS-Plus/index.html \n
    https://github.com/FreeRTOS
</tr>
</table>

## See also

\ref KERNEL_DPL_PAGE, \ref KERNEL_NORTOS_PAGE
