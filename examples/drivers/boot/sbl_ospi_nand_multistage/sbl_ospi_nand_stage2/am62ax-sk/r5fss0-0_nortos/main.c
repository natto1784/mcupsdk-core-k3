/*
 *  Copyright (C) 2018-2024 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <drivers/device_manager/sciclient.h>
#include <drivers/bootloader.h>
#include <drivers/pinmux.h>
#include <drivers/gtc.h>
#include <kernel/dpl/CacheP.h>


/* This start address and length depends upon the linker memory for second stage SBL.
   It is necessary to change the below start address and length if in case the linker
   memory region for second stage SBL is changed.
*/

#define BOOTLOADER_SECOND_STAGE_RESERVED_MEMORY_START       0xB0340000
#define BOOTLOADER_SECOND_STAGE_RESERVED_MEMORY_LENGTH      0x200000

CacheP_Config gCacheConfig = {};

/* This buffer needs to be defined for OSPI nand boot in case of HS device for
   image authentication
   The size of the buffer should be large enough to accomodate the appimage */
uint8_t gAppimage[0x2000000] __attribute__ ((section (".bss.app"), aligned (128)));

/*  In this sample bootloader, we load appimages for RTOS/Baremetal at different offset
    i.e the appimage for RTOS/Baremetal (for R5, MCU R5, A53) is flashed at different offset in eMMC

    Here at one eMMC offset, there is a multi-core .appimage that holds RPRC for MCU R5 and R5
    and another .appimage that holds the A53 binaries at another offset.

    When flashing make sure to flash images to below offset using the flash tool.

    RTOS/Baremetal appimage (for HSM) flash at offset 0x800000 of flash
    RTOS/Baremetal appimage (for A53) flash at offset 0xC00000 of flash
*/

/* call this API to stop the booting process and spin, do that you can connect
 * debugger, load symbols and then make the 'loop' variable as 0 to continue execution
 * with debugger connected.
 */
void loop_forever()
{
    volatile uint32_t loop = 1;
    while(loop)
        ;
}

void App_loadImages(Bootloader_LoadImageParams *bootLoadParams)
{
	int32_t status = SystemP_FAILURE;
    Bootloader_Config *bootConfig;

    if(bootLoadParams->bootHandle  != NULL)
    {
        bootConfig = (Bootloader_Config *)bootLoadParams->bootHandle;
        bootConfig->coresPresentMap = 0;
        status = Bootloader_parseMultiCoreAppImage(bootLoadParams->bootHandle, &bootLoadParams->bootImageInfo);

        /* Load CPUs */
        if((status == SystemP_SUCCESS) && TRUE == Bootloader_isCorePresent(bootLoadParams->bootHandle, CSL_CORE_ID_HSM_M4FSS0_0))
        {
            (&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_HSM_M4FSS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_HSM_M4FSS0_0);
            Bootloader_profileAddCore(CSL_CORE_ID_HSM_M4FSS0_0);
            status = Bootloader_loadCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_HSM_M4FSS0_0]));
            if(status == SystemP_SUCCESS)
            {
                Bootloader_profileAddProfilePoint("HSM Image Load");
                bootLoadParams->coreId  = CSL_CORE_ID_HSM_M4FSS0_0;
                bootLoadParams->loadStatus  = BOOTLOADER_IMAGE_LOADED;
            }
            else
            {
                Bootloader_powerOffCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_HSM_M4FSS0_0]));
            }
            return;
        }

        if((SystemP_SUCCESS == status) && (TRUE == Bootloader_isCorePresent(bootLoadParams->bootHandle, CSL_CORE_ID_C75SS0_0)))
        {
            (&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_C75SS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_C75SS0_0);
            Bootloader_profileAddCore(CSL_CORE_ID_C75SS0_0);
            status = Bootloader_loadCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_C75SS0_0]));
            if(status == SystemP_SUCCESS)
            {
                Bootloader_profileAddProfilePoint("DSP Image Load");
                bootLoadParams->coreId  = CSL_CORE_ID_C75SS0_0;
                bootLoadParams->loadStatus  = BOOTLOADER_IMAGE_LOADED;
            }
            else
            {
                Bootloader_powerOffCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_C75SS0_0]));
            }
            return;
        }

        if((SystemP_SUCCESS == status) && (TRUE == Bootloader_isCorePresent(bootLoadParams->bootHandle, CSL_CORE_ID_A53SS0_0)))
		{
			if((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS0_0].smpEnable == true)
            {
                (&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS0_0);
                (&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS0_1].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS0_1);
                (&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS1_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS1_0);
                (&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS1_1].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS1_1);

                Bootloader_profileAddCore(CSL_CORE_ID_A53SS0_0);
                Bootloader_profileAddCore(CSL_CORE_ID_A53SS0_1);
                Bootloader_profileAddCore(CSL_CORE_ID_A53SS1_0);
                Bootloader_profileAddCore(CSL_CORE_ID_A53SS1_1);
                status = Bootloader_loadCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS0_0]));
            }
            else
            {
			    (&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS0_0);
			    Bootloader_profileAddCore(CSL_CORE_ID_A53SS0_0);
                status = Bootloader_loadCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS0_0]));
            }
            if(status == SystemP_SUCCESS)
            {
                Bootloader_profileAddProfilePoint("A53 Image Load");
                bootLoadParams->coreId  = CSL_CORE_ID_A53SS0_0;
                bootLoadParams->loadStatus  = BOOTLOADER_IMAGE_LOADED;
            }
            else
            {
                Bootloader_powerOffCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS0_0]));
            }
            return;
        }

        if((SystemP_SUCCESS == status) && (TRUE == Bootloader_isCorePresent(bootLoadParams->bootHandle, CSL_CORE_ID_R5FSS0_0)))
        {
            (&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_R5FSS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_R5FSS0_0);
            Bootloader_profileAddCore(CSL_CORE_ID_R5FSS0_0);
            status = Bootloader_loadSelfCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_R5FSS0_0]));
            if(status == SystemP_SUCCESS)
            {
                Bootloader_profileAddProfilePoint("DM R5 Image Load");
                bootLoadParams->coreId  = CSL_CORE_ID_R5FSS0_0;
                bootLoadParams->loadStatus  = BOOTLOADER_IMAGE_LOADED;
            }
            return;
        }
    }
}

void App_runCpus(Bootloader_LoadImageParams *bootLoadParams)
{
    int32_t status = SystemP_FAILURE;
    int8_t coreId = bootLoadParams->coreId;

    if(coreId != CSL_CORE_ID_R5FSS0_0)
    {
        if(bootLoadParams->loadStatus == BOOTLOADER_IMAGE_LOADED)
        {
	        status = Bootloader_runCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[coreId]));
            if(status == SystemP_FAILURE)
            {
                Bootloader_powerOffCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[coreId]));
            }

            if(((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS0_0].smpEnable == true) && status == SystemP_SUCCESS)
            {
                Bootloader_runCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS0_1]));
                Bootloader_runCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS1_0]));
                Bootloader_runCpu(bootLoadParams->bootHandle, &((&bootLoadParams->bootImageInfo)->cpuInfo[CSL_CORE_ID_A53SS1_1]));
            }

            Bootloader_close(bootLoadParams->bootHandle);
        }
    }
}

int main()
{
    int32_t status;

    Bootloader_profileReset();

    System_init();
    Bootloader_profileAddProfilePoint("System_init");

    Board_init();
    Bootloader_profileAddProfilePoint("Board_init");

    Drivers_open();
    Bootloader_profileAddProfilePoint("Drivers_open");

    status = Board_driversOpen();
    DebugP_assert(status == SystemP_SUCCESS);
    Bootloader_profileAddProfilePoint("Board_driversOpen");

    status = Sciclient_getVersionCheck(1);
    Bootloader_profileAddProfilePoint("Sciclient Get Version");

    Bootloader_ReservedMemInit(BOOTLOADER_SECOND_STAGE_RESERVED_MEMORY_START, \
                                BOOTLOADER_SECOND_STAGE_RESERVED_MEMORY_LENGTH);

    if(SystemP_SUCCESS == status)
    {
        Bootloader_LoadImageParams bootArray[CONFIG_BOOTLOADER_NUM_INSTANCES];
        uint32_t imageSize = 0;

        for(uint8_t inst = 0; inst < CONFIG_BOOTLOADER_NUM_INSTANCES; inst++)
        {
            Bootloader_Params_init(&bootArray[inst].bootParams);
            Bootloader_BootImageInfo_init(&bootArray[inst].bootImageInfo);
            bootArray[inst].bootHandle = Bootloader_open(inst, &bootArray[inst].bootParams);
            bootArray[inst].loadStatus = BOOTLOADER_IMAGE_NOT_LOADED;
            if(bootArray[inst].bootHandle != NULL)
            {
               ((Bootloader_Config *)bootArray[inst].bootHandle)->scratchMemPtr = gAppimage;
		    	App_loadImages(&bootArray[inst]);
            }
            imageSize += Bootloader_getMulticoreImageSize(bootArray[inst].bootHandle);
        }

        Bootloader_profileUpdateAppimageSize(imageSize);
        Bootloader_profileUpdateMediaAndClk(BOOTLOADER_MEDIA_FLASH, OSPI_getInputClk(gOspiHandle[CONFIG_OSPI0]));

		/* Print SBL log to the same UART port */
		Bootloader_profilePrintProfileLog();
		DebugP_log("Image loading done, switching to application ...\r\n");
		DebugP_log("Starting RTOS/Baremetal applications\r\n");
		UART_flushTxFifo(gUartHandle[CONFIG_UART0]);

        /* Deinitialise the flash peripherial before starting other cores,
           so that other systems can access and reinitialise it.*/
        Board_driversClose();
        Drivers_close();

		if(bootArray[CONFIG_BOOTLOADER_FLASH_DM].loadStatus == BOOTLOADER_IMAGE_LOADED)
        {
            for(uint8_t inst = 0; inst < CONFIG_BOOTLOADER_NUM_INSTANCES; inst++)
            {
		        App_runCpus(&bootArray[inst]);
            }
        }
        else
        {
            status = SystemP_FAILURE;
        }
    }

    if(status != SystemP_SUCCESS )
    {
        DebugP_log("DM image not loaded\r\n");
    }

    Bootloader_JumpSelfCpu();

    Board_deinit();
    System_deinit();

    return 0;
}
