/*
 *  Copyright (C) 2023-2024 Texas Instruments Incorporated
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <board/ioexp/ioexp_tca6424.h>
#include <kernel/dpl/CacheP.h>
#include <drivers/device_manager/sciclient.h>
#include <drivers/sciclient/include/tisci/security/tisci_ext_otp.h>
#include "ext_otp.h"
#include <stdint.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* VPP CORE on AM62X EVM */
#define EFUSE_VPP_PIN (4U)
#define EFUSE_VPP_PIN_LED (23U) /* test LED */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static CSL_wkup_ctrl_mmr_cfg0Regs * gCtrlMmrPtr = (CSL_wkup_ctrl_mmr_cfg0Regs *) CSL_WKUP_CTRL_MMR0_CFG0_BASE;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void ext_otp_setVpp(void)
{
	int32_t status ;

	TCA6424_Params TCA6424_IOexp_params =
	{
		.i2cInstance = 0,
		.i2cAddress = 0x22
	};

	TCA6424_Config TCA6424_IOexp_config ;

    status = TCA6424_open(&TCA6424_IOexp_config, &TCA6424_IOexp_params);

	/* set VPP core */
    if (status == SystemP_SUCCESS)
	{
		status = TCA6424_config(&TCA6424_IOexp_config, EFUSE_VPP_PIN, TCA6424_MODE_OUTPUT);
	}

    if (status == SystemP_SUCCESS)
	{
    	status = TCA6424_setOutput(&TCA6424_IOexp_config, EFUSE_VPP_PIN, TCA6424_OUT_STATE_HIGH);
	}

	/* make onboard RED LED on indicating VPP is set. */
    if (status == SystemP_SUCCESS)
	{
		status = TCA6424_config(&TCA6424_IOexp_config, EFUSE_VPP_PIN_LED, TCA6424_MODE_OUTPUT);
	}

    if (status == SystemP_SUCCESS)
	{
    	status = TCA6424_setOutput(&TCA6424_IOexp_config, EFUSE_VPP_PIN_LED, TCA6424_OUT_STATE_HIGH);
	}

    TCA6424_close(&TCA6424_IOexp_config);

	DebugP_assertNoLog(status==SystemP_SUCCESS);
}

/**
 *  \brief Function to read OTP MMR.
 *  	   Note: Host should have read permission to row.
 *  	   Read permissions to perticular row can be set via
 *  	   board configuration security.
 *
 *  \param mmrIdx   [IN] MMR index to read [0,31]
 *  \param mmrVal   [IN] Pointer to store read value
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_readMmr(uint8_t mmrIdx, uint32_t *mmrVal)
{
    int32_t status = SystemP_SUCCESS;
    Sciclient_ReqPrm_t reqParam;
    Sciclient_RespPrm_t respParam;
    struct tisci_msg_read_otp_mmr_req request;
    struct tisci_msg_read_otp_mmr_resp response;

    request.mmr_idx           = mmrIdx;

    reqParam.messageType      = (uint16_t) TISCI_MSG_READ_OTP_MMR;
    reqParam.flags            = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload      = (const uint8_t *) &request;
    reqParam.reqPayloadSize   = (uint32_t) sizeof (request);
    reqParam.timeout          = (uint32_t) SystemP_WAIT_FOREVER;

    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t) sizeof (response);

    status = Sciclient_service(&reqParam, &respParam);

	if ( (status==SystemP_SUCCESS) && ((respParam.flags & TISCI_MSG_FLAG_ACK) == TISCI_MSG_FLAG_ACK) )
	{
		*mmrVal = response.mmr_val;
		DebugP_logInfo("Success Reading OTP MMR \r\n");
		DebugP_logInfo("OTP MMR Value reported :0x%x \r\n", (uint32_t) *mmrVal);
	}
	else
	{
		DebugP_logError("Error Reading OTP MMR ... \r\n");
		status = SystemP_FAILURE;
	}

	return status;
}

/**
 *  \brief Function to write particular row of EFUSE. Note that VPP must be set
 *  	   high before calling this function, in order to writes to be successful.
 *
 *  \param row_idx   [IN] row id of 25bit row
 *  \param row_val   [IN] value to write in row
 *  \param row_mask  [IN] mask to write. Bits corresponding to 1 in mask will be written.
 *  \param rowValRdBk  [IN] readback value after writing
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_writeRow(uint8_t rowIdx, uint32_t rowVal, uint32_t rowMask, uint32_t *rowValRdBk )
{
    int32_t status = SystemP_SUCCESS;
    Sciclient_ReqPrm_t reqParam;
    Sciclient_RespPrm_t respParam;
    struct tisci_msg_write_otp_row_req request;
    struct tisci_msg_write_otp_row_resp response;

    request.row_idx           = rowIdx;
    request.row_val           = rowVal;
    request.row_mask          = rowMask;

    reqParam.messageType      = (uint16_t) TISCI_MSG_WRITE_OTP_ROW;
    reqParam.flags            = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload      = (const uint8_t *) &request;
    reqParam.reqPayloadSize   = (uint32_t) sizeof (request);
    reqParam.timeout          = (uint32_t) SystemP_WAIT_FOREVER;

    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t) sizeof (response);

    status = Sciclient_service(&reqParam, &respParam);

	if ( (status==SystemP_SUCCESS) && ((respParam.flags & TISCI_MSG_FLAG_ACK) == TISCI_MSG_FLAG_ACK) )
	{
		DebugP_logInfo("Success writing OTP row %d\r\n", rowIdx);
		*rowValRdBk = response.row_val;
	}
	else
	{
		DebugP_logError("Error writing OTP row %d \r\n", rowIdx);
		status = SystemP_FAILURE;
	}

	return status;
}

static uint32_t ext_otp_getMaskSize32(uint32_t mask)
{
	/* Count number of ones */
	uint32_t count = 32U;
	uint32_t maskSize = 0;

	while(count--)
	{
		maskSize += (mask & 1U);
		mask >>= 1;
	}

	return maskSize;
}

/**
 *  \brief Function to write GP EXT OTP MMR. Note that VPP must be
 *  	   set to high while calling this function.
 *
 *  \param mmrIdx   [IN] mmr id [0,31]
 *  \param mmrVal   [IN] Value to write on OTP MMR
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_writeMmr(uint32_t mmrIdx, uint32_t mmrVal)
{
	int32_t status = SystemP_SUCCESS;
	uint32_t rowStart, rowColStart;
	uint32_t rowEnd, rowColEnd, rowCount;
	uint32_t maskRow1, maskRow2, maskRow3;
	uint32_t valRow1, valRow2, valRow3;
	uint32_t rowValRdBk;

	/* Bit position in this start row  */
	rowColStart = ((mmrIdx * MMR_SIZE_BITS) % NUM_BITS_PER_OTP_ROW) + OTP_COL_OFFSET;
    if (rowColStart >= NUM_BITS_PER_OTP_ROW)
    {
        rowColStart %= NUM_BITS_PER_OTP_ROW;
        /* EFUSE row start for this MMR */
	    rowStart = ((mmrIdx * MMR_SIZE_BITS) / NUM_BITS_PER_OTP_ROW) + 1U;
    }
    else
    {
        /* EFUSE row start for this MMR */
	    rowStart = (mmrIdx * MMR_SIZE_BITS) / NUM_BITS_PER_OTP_ROW;
    }
	rowCount = (rowColStart + MMR_SIZE_BITS) / NUM_BITS_PER_OTP_ROW;
	if ( ((rowColStart + MMR_SIZE_BITS) % NUM_BITS_PER_OTP_ROW) != 0 )
	{
		/* Third row needed */
		rowCount += 1;
	}
	/* Total rowCount number of rows needed */
	rowEnd = rowStart + rowCount - 1;
	rowColEnd = ((rowColStart + MMR_SIZE_BITS) % NUM_BITS_PER_OTP_ROW) - 1;

	if (rowCount == 2)
	{
		maskRow1 = (~((1 << rowColStart) - 1)) & OTP_ROW_FULL_MASK; /* for rowStart */
		maskRow2 = ((1 << (rowColEnd + 1)) - 1) & OTP_ROW_FULL_MASK; /* for rowEnd */

		valRow1 = ( (1 << ext_otp_getMaskSize32(maskRow1)) - 1 ) & mmrVal;
		mmrVal >>= ext_otp_getMaskSize32(maskRow1);

		valRow2 = ( (1 << ext_otp_getMaskSize32(maskRow2)) - 1 ) & mmrVal;
		mmrVal >>= ext_otp_getMaskSize32(maskRow2);

		/* MMR EXT OTP efuse writing */
		DebugP_logInfo("row=%d val= 0x%x mask=0x%x \r\n",rowStart, (valRow1 << rowColStart), maskRow1);
		DebugP_logInfo("row=%d val= 0x%x mask=0x%x \r\n",rowEnd, (valRow2 << 0), maskRow2);

		status =  ext_otp_writeRow(rowStart, (valRow1 << rowColStart), maskRow1, &rowValRdBk);

		if (status == SystemP_SUCCESS)
		{
			status =  ext_otp_writeRow(rowEnd, (valRow2 << 0), maskRow2, &rowValRdBk);
		}
	}
	else if (rowCount == 3)
	{
		maskRow1 = (~((1 << rowColStart) - 1)) & OTP_ROW_FULL_MASK;
		maskRow2 = OTP_ROW_FULL_MASK;
		maskRow3 = ((1 << (rowColEnd + 1)) - 1) & OTP_ROW_FULL_MASK;


		valRow1 = ( (1 << ext_otp_getMaskSize32(maskRow1)) - 1 ) & mmrVal;
		mmrVal >>= ext_otp_getMaskSize32(maskRow1);

		valRow2 = ( (1 << ext_otp_getMaskSize32(maskRow2)) - 1 ) & mmrVal;
		mmrVal >>= ext_otp_getMaskSize32(maskRow2);

		valRow3 = ( (1 << ext_otp_getMaskSize32(maskRow3)) - 1 ) & mmrVal;
		mmrVal >>= ext_otp_getMaskSize32(maskRow3);

		DebugP_logInfo("row=%d: val= 0x%x mask=0x%x \r\n",rowStart, (valRow1 << rowColStart), maskRow1);
		DebugP_logInfo("row=%d: val= 0x%x mask=0x%x \r\n",rowStart + 1, (valRow2 << 0), maskRow2);
		DebugP_logInfo("row=%d: val= 0x%x mask=0x%x \r\n",rowEnd, (valRow3 << 0), maskRow3);

		status =  ext_otp_writeRow(rowStart, (valRow1 << rowColStart), maskRow1, &rowValRdBk );
		if (status == SystemP_SUCCESS)
		{
			status =  ext_otp_writeRow(rowStart + 1, (valRow2 << 0), maskRow2, &rowValRdBk );
		}
		if (status == SystemP_SUCCESS)
		{
			status =  ext_otp_writeRow(rowEnd, (valRow3 << 0), maskRow3, &rowValRdBk );
		}
	}

	DebugP_logInfo("mmrIdx: %d,rowStart: %d, rowColStart: %d, rowEnd: %d, rowColEnd: %d \r\n", mmrIdx, rowStart, rowColStart, rowEnd, rowColEnd);

	return status;
}

/**
 *  \brief Function to write magic words and configure EXT OTP area as expected
 *  	   by ROM. Once this area is configured. ROM reads programmed USB VID/PID
 *  	  from sec manager to updates in control MMR on each PORZ.
 *
 *  \return status [out] SystemP_SUCCESS on success
 *
 */
static int32_t ext_otp_setRomConfig()
{
	int32_t status = SystemP_FAILURE;
	uint32_t mmrValIDConfig = 0;

    /* ID0 : USB VID */
	/* ID1 : USB PID */
	CSL_REG32_FINS_RAW(&mmrValIDConfig, EXT_OTP_ROM_MAGIC_WORD_MASK, EXT_OTP_ROM_MAGIC_WORD_SHIFT, EXT_OTP_ROM_MAGIC_WORD);
	CSL_REG32_FINS_RAW(&mmrValIDConfig, EXT_OTP_ID0_FLAG_MASK, EXT_OTP_ID0_FLAG_SHIFT, EXT_OTP_FLAG_USB_VID);
	CSL_REG32_FINS_RAW(&mmrValIDConfig, EXT_OTP_ID1_FLAG_MASK, EXT_OTP_ID1_FLAG_SHIFT, EXT_OTP_FLAG_USB_PID);

	status = ext_otp_writeMmr((EXT_OTP_MAX_MMRS - 1U), mmrValIDConfig);

	return status;
}

/**
 *  \brief Function to program USB VID/PID in Efuses. ROM loads these values in
 *  	   control MMRs upon PORZ.
 *
 *  \param usb_vid   [IN] USB Vendor ID
 *  \param usb_pid   [IN] USB PID
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_writeUsbVidPid(uint32_t usbVID, uint32_t usbPID)
{
	int32_t status = SystemP_FAILURE;
	uint32_t  mmrData = 0;

	status = ext_otp_setRomConfig();

	/* Write Data OTP MMR 30 */
	CSL_REG32_FINS_RAW(&mmrData, EXT_OTP_ID0_DATA_MASK, EXT_OTP_ID0_DATA_SHIFT, usbVID);
	CSL_REG32_FINS_RAW(&mmrData, EXT_OTP_ID1_DATA_MASK, EXT_OTP_ID1_DATA_SHIFT, usbPID);

	/* Write into efuse */
	status = ext_otp_writeMmr((EXT_OTP_MAX_MMRS - 2U), mmrData);

	return status;
}

/**
 *  \brief Function to print all OTP MMR values.
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_printMmrs(void)
{
	int32_t status = SystemP_SUCCESS;
	uint32_t mmrVal, i;
	for (i = 0; i < EXT_OTP_MAX_MMRS; i++)
	{
		if (status == SystemP_SUCCESS)
		{
			status = ext_otp_readMmr(i, &mmrVal);
			DebugP_log("OTP MMR %d: 0x%x \r\n", i, mmrVal);
		}
		else
		{
			DebugP_log("Error reading MMR %d \r\n", i);
		}
	}

	return status;
}

/**
 *  \brief Function to read USB VID from CONTROL MMR
 *
 *  \param usb_vid   [IN] pointer to store VID read.
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_getUsbVid(uint32_t *usb_vid)
{
	int32_t status = SystemP_SUCCESS;
	*usb_vid = ( gCtrlMmrPtr->USB_DEVICE_ID0 & CTRL_MMR_USB_VID_MASK ) >> CTRL_MMR_USB_VID_SHIFT;
	return status;
}

/**
 *  \brief Function to read USB PID from CONTROL MMR
 *
 *  \param usb_pid   [IN] pointer to store PID read.
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_getUsbPid(uint32_t *usb_pid)
{
	int32_t status = SystemP_SUCCESS;
	*usb_pid = ( gCtrlMmrPtr->USB_DEVICE_ID0 & CTRL_MMR_USB_PID_MASK ) >> CTRL_MMR_USB_PID_SHIFT;
	return status;
}

/**
 *  \brief Function to permanently lock particular row of EFUSE. This prevents any
 *         further modifications to the row even after a device cold/warm reset.Note
 *         that VPP must be set high before calling this function, in order to writes
 *         to be successful.
 *
 *  \param rowIdx   [IN] row id of 25bit row
 *  \param hwWriteLock   [IN] indicates if write lock has to be applied in HW on the current row. Set to 0x5A for write lock.
 *  \param hwReadLock  [IN] indicates if read lock has to be applied in HW on the current row. Set to 0x5A for read lock.
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_lockRow(uint8_t rowIdx, uint8_t hwWriteLock, uint8_t hwReadLock)
{
    int32_t status = SystemP_SUCCESS;
    Sciclient_ReqPrm_t reqParam;
    Sciclient_RespPrm_t respParam;
    struct tisci_msg_lock_otp_row_req request;
    struct tisci_msg_lock_otp_row_resp response;

    request.row_idx           = rowIdx;
    request.hw_write_lock     = hwWriteLock;
    request.hw_read_lock      = hwReadLock;

    reqParam.messageType      = (uint16_t) TISCI_MSG_LOCK_OTP_ROW;
    reqParam.flags            = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload      = (const uint8_t *) &request;
    reqParam.reqPayloadSize   = (uint32_t) sizeof (request);
    reqParam.timeout          = (uint32_t) SystemP_WAIT_FOREVER;

    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t) sizeof (response);

    status = Sciclient_service(&reqParam, &respParam);

	if ( (status==SystemP_SUCCESS) && ((respParam.flags & TISCI_MSG_FLAG_ACK) == TISCI_MSG_FLAG_ACK) )
	{
		DebugP_logInfo("Success locking OTP row %d\r\n", rowIdx);\
	}
	else
	{
		DebugP_logError("Error locking OTP row %d \r\n", rowIdx);
		status = SystemP_FAILURE;
	}

	return status;
}

/**
 *  \brief Function to get the lock status of particular row of EFUSE. Note
 *         that VPP must be set high before calling this function, in order
 *         to writes to be successful.
 *
 *  \param rowIdx   [IN] row id of 25bit row
 *  \param globalsoftLock  [IN] pointer to store global soft lock value
 *  \param hwWriteLock   [IN] pointer to store row write lock value
 *  \param hwReadLock  [IN] pointer to store row read lock value
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_getRowLockStatus(uint8_t rowIdx, uint8_t *globalsoftLock, uint8_t *hwWriteLock, uint8_t *hwReadLock)
{
    int32_t status = SystemP_SUCCESS;
    Sciclient_ReqPrm_t reqParam;
    Sciclient_RespPrm_t respParam;
    struct tisci_msg_get_otp_row_lock_status_req request;
    struct tisci_msg_get_otp_row_lock_status_resp response;

    request.row_idx           = rowIdx;

    reqParam.messageType      = (uint16_t) TISCI_MSG_GET_OTP_ROW_LOCK_STATUS;
    reqParam.flags            = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload      = (const uint8_t *) &request;
    reqParam.reqPayloadSize   = (uint32_t) sizeof (request);
    reqParam.timeout          = (uint32_t) SystemP_WAIT_FOREVER;

    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t) sizeof (response);

    status = Sciclient_service(&reqParam, &respParam);

	if ( (status==SystemP_SUCCESS) && ((respParam.flags & TISCI_MSG_FLAG_ACK) == TISCI_MSG_FLAG_ACK) )
	{
		DebugP_logInfo("Success getting lock status of OTP row %d\r\n", rowIdx);
        *globalsoftLock = response.global_soft_lock;
        *hwWriteLock = response.hw_write_lock;
        *hwReadLock = response.hw_read_lock;
	}
	else
	{
		DebugP_logError("Error getting lock status of OTP row %d \r\n", rowIdx);
		status = SystemP_FAILURE;
	}

	return status;
}

/**
 *  \brief Function to perform a global soft write lock on OTP rows. This prevents
 *         further writes to OTP until a warm/cold reset of the device. Note that
 *         VPP must be set high before calling this function, in order to writes
 *         to be successful.
 *
 *  \return status [out] SystemP_SUCCESS on success
 */
int32_t ext_otp_softLockWriteGlobal(void)
{
    int32_t status = SystemP_SUCCESS;
    Sciclient_ReqPrm_t reqParam;
    Sciclient_RespPrm_t respParam;
    struct tisci_msg_soft_lock_otp_write_global_req request;
    struct tisci_msg_soft_lock_otp_write_global_resp response;

    reqParam.messageType      = (uint16_t) TISCI_MSG_SOFT_LOCK_OTP_WRITE_GLOBAL;
    reqParam.flags            = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload      = (const uint8_t *) &request;
    reqParam.reqPayloadSize   = (uint32_t) sizeof (request);
    reqParam.timeout          = (uint32_t) SystemP_WAIT_FOREVER;

    respParam.flags           = (uint32_t) 0U;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t) sizeof (response);

    status = Sciclient_service(&reqParam, &respParam);

	if ( (status==SystemP_SUCCESS) && ((respParam.flags & TISCI_MSG_FLAG_ACK) == TISCI_MSG_FLAG_ACK) )
	{
		DebugP_logInfo("Success global soft write lock of OTP rows \r\n");
	}
	else
	{
		DebugP_logError("Error global soft write lock of OTP rows \r\n");
		status = SystemP_FAILURE;
	}

	return status;
}
