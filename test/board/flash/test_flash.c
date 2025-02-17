/*
 *  Copyright (C) 2023 Texas Instruments Incorporated
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

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <unity.h>
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define TEST_FLASH_OFFSET_BASE      (0x200000U)

#if defined (SOC_AM62AX) || defined (SOC_AM62X)
#define TEST_FLASH_PAGE_SIZE        (4096)
#define TEST_FLASH_BLOCK_SIZE       (262144)
#define TEST_FLASH_DATA_MAX_VALUE   (256)
#define TEST_FLASH_BUF_LEN          (15U)
#define TEST_FLASH_BYTE_OFFSET      (8U)
#define TEST_FLASH_TEMP_BUF_SIZE    (32U)
#define TEST_FLASH_BUF_LEN_DMA      (2024U)
#else
#define TEST_FLASH_DATA_SIZE        (256U)
#define TEST_FLASH_RX_BUF_SIZE      (2048U)
#define TEST_FLASH_BUF_LEN_ODD      (15U)
#define TEST_FLASH_BYTE_OFFSET_ODD  (7U)
#define TEST_FLASH_TEMP_BUF_SIZE    (32U)
#define TEST_FLASH_BUF_LEN_ODD_DMA  (2021U)
#endif
/* ========================================================================== */
/*                             Global Variables                               */
/* ========================================================================== */
#if defined(SOC_AM62AX) || defined (SOC_AM62X)
uint8_t gFlashTestTxBuf[TEST_FLASH_BLOCK_SIZE];
uint8_t gFlashTestRxBuf[TEST_FLASH_BLOCK_SIZE]; // __attribute__((aligned(128U)));
#else
uint8_t gFlashTestTxBuf[TEST_FLASH_DATA_SIZE] =
{
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
};

uint8_t gFlashTestRxBuf[TEST_FLASH_RX_BUF_SIZE] __attribute__((aligned(128U)));
#endif
/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* Testcases */
#if defined(SOC_AM62AX) || defined (SOC_AM62X)
static void test_nand_flash_readwrite(void *args);
static void test_nand_flash_read_multiple(void *args);
static void test_nand_flash_skip_bad_block(void *args);
#else
static void test_flash_readwrite(void *args);
static void test_flash_read_multiple(void *args);
#endif



/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void test_main(void *args)
{
    Board_flashClose();
    Drivers_ospiClose();
    Drivers_ospiOpen();

    UNITY_BEGIN();
#if defined(SOC_AM62AX) || defined (SOC_AM62X)
    RUN_TEST(test_nand_flash_readwrite, 246, NULL);
#else
    RUN_TEST(test_flash_readwrite, 246, NULL);
#endif

#if defined (SOC_AM273X) || defined (SOC_AWR294X) || defined (SOC_AM263X)
    Drivers_qspiClose();
    Drivers_qspiOpen();
#else
    Drivers_ospiClose();
    Drivers_ospiOpen();
#endif
#if defined(SOC_AM62AX) || defined (SOC_AM62X)
    RUN_TEST(test_nand_flash_read_multiple, 247, NULL);
    RUN_TEST(test_nand_flash_skip_bad_block, 4852, NULL);
#else
    RUN_TEST(test_flash_read_multiple, 247, NULL);
#endif

    UNITY_END();

    return;
}

/* Unity framework required information */
void setUp(void)
{
}

void tearDown(void)
{
}

/* Testcases */
#if defined(SOC_AM62AX) || defined (SOC_AM62X)
static void test_nand_flash_readwrite(void *args)
{
    int32_t retVal = SystemP_SUCCESS;
    uint32_t blk, offset, page;

    /* Open Flash drivers with OSPI instance as input */
    retVal = Board_driversOpen();
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Flash_Attrs *flashAttrs = Flash_getAttrs(CONFIG_FLASH0);

    uint16_t uOffset = 0;
    uint16_t uValue;

    /* Fill the gFlashTestTxBuf with values 0-255 */
    while(uOffset < TEST_FLASH_PAGE_SIZE)
    {
        for(uValue = 0; uValue < TEST_FLASH_DATA_MAX_VALUE; uValue++)
        {
            gFlashTestTxBuf[uOffset ++] = uValue;
        }
    }

    uint32_t loopCnt = 10U;
    uint32_t i;

    /* Set an invalid block count */
    uint32_t currBlock = flashAttrs->blockCount + 1;

    for(i=0; i<loopCnt; i++)
    {
        memset(gFlashTestRxBuf, '\0', TEST_FLASH_PAGE_SIZE);
        offset = TEST_FLASH_OFFSET_BASE + i*(flashAttrs->pageSize*flashAttrs->pageCount);
        Flash_offsetToBlkPage(gFlashHandle[CONFIG_FLASH0], offset, &blk, &page);
        if(currBlock != blk)
        {
            Flash_eraseBlk(gFlashHandle[CONFIG_FLASH0], blk);
            currBlock = blk;
        }
        Flash_write(gFlashHandle[CONFIG_FLASH0], offset, gFlashTestTxBuf, TEST_FLASH_PAGE_SIZE);
        Flash_read(gFlashHandle[CONFIG_FLASH0], offset, gFlashTestRxBuf, TEST_FLASH_PAGE_SIZE);
        retVal |= memcmp(gFlashTestTxBuf, gFlashTestRxBuf, TEST_FLASH_PAGE_SIZE);
    }
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Board_driversClose();
}
static void test_nand_flash_read_multiple(void *args)
{
    int32_t retVal = SystemP_SUCCESS;
    uint32_t blk, offset, page;

    /* Open Flash drivers with OSPI instance as input */
    retVal = Board_driversOpen();
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Flash_Attrs *flashAttrs = Flash_getAttrs(CONFIG_FLASH0);

    /* Write two blocks of data (512 KB)*/
    uint32_t loopCnt = 2 * flashAttrs->pageCount;
    uint32_t i;

    memset(gFlashTestRxBuf, '\0', TEST_FLASH_PAGE_SIZE);

    /* Set an invalid block count */
    uint32_t currBlock = flashAttrs->blockCount + 1;

    for(i=0; i<loopCnt; i++)
    {
        offset = TEST_FLASH_OFFSET_BASE + i*(flashAttrs->pageSize);
        Flash_offsetToBlkPage(gFlashHandle[CONFIG_FLASH0], offset, &blk, &page);
        if(currBlock != blk)
        {
            Flash_eraseBlk(gFlashHandle[CONFIG_FLASH0], blk);
            currBlock = blk;
        }
        Flash_write(gFlashHandle[CONFIG_FLASH0], offset, gFlashTestTxBuf, TEST_FLASH_PAGE_SIZE);
    }

    /* READ CHECKS */

    uint8_t recvBuf[TEST_FLASH_TEMP_BUF_SIZE] __attribute__((aligned(128U)));
    uint8_t checkBuf[TEST_FLASH_TEMP_BUF_SIZE];
    uint32_t checkIndex = 0U;

    /*
     * CHECK 1 : Read odd bytes from an aligned offset
     */
    /* Set an aligned offset */
    offset = TEST_FLASH_OFFSET_BASE + 4096;

    /* Fill the check buffer */
    for(i = 0; i < TEST_FLASH_BUF_LEN; i++)
    {
        checkBuf[i] = gFlashTestTxBuf[(offset+i)%TEST_FLASH_DATA_MAX_VALUE];
    }

    /* Read the bytes from the aligned offset */
    memset(recvBuf, 0, TEST_FLASH_TEMP_BUF_SIZE);
    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, recvBuf, TEST_FLASH_BUF_LEN);

    retVal = memcmp(checkBuf, recvBuf, TEST_FLASH_BUF_LEN);
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    /*
     * CHECK 2 : Read  bytes from unaligned offset
     */
    /* Set an unaligned offset(offset should be 8byte alligned) */
    offset = TEST_FLASH_OFFSET_BASE + 104;

    /* Fill the check buffer */
    for(i = 0; i < TEST_FLASH_BUF_LEN; i++)
    {
        checkBuf[i] = gFlashTestTxBuf[(offset+i)%TEST_FLASH_PAGE_SIZE];
    }
    /* Read the bytes from the unaligned offset */
    memset(recvBuf, 0, TEST_FLASH_TEMP_BUF_SIZE);
    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, recvBuf, TEST_FLASH_BUF_LEN);

    retVal = memcmp(checkBuf, recvBuf, TEST_FLASH_BUF_LEN);
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    /*
     * CHECK 3: Read across page boundary
     */
    /* Set offset back some  bytes */
    offset = TEST_FLASH_OFFSET_BASE + TEST_FLASH_PAGE_SIZE - TEST_FLASH_BYTE_OFFSET;

    /* Expected index in check buffer*/
    checkIndex = TEST_FLASH_PAGE_SIZE - TEST_FLASH_BYTE_OFFSET;

    /* Fill the check buffer */
    for(i = 0; i < TEST_FLASH_BUF_LEN; i++)
    {
        checkBuf[i] = gFlashTestTxBuf[(checkIndex+i)%TEST_FLASH_PAGE_SIZE];
    }

    /* Read the bytes from the offset across the pages */
    memset(recvBuf, 0, TEST_FLASH_TEMP_BUF_SIZE);
    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, recvBuf, TEST_FLASH_BUF_LEN);

    retVal = memcmp(checkBuf, recvBuf, TEST_FLASH_BUF_LEN);
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    /*
     * CHECK 4 : Read across block boundary
     */
    /* Set offset back some  bytes */
    offset = TEST_FLASH_OFFSET_BASE + flashAttrs->pageCount*flashAttrs->pageSize - TEST_FLASH_BYTE_OFFSET;

    /* Expected index in test buffer*/
    checkIndex = TEST_FLASH_PAGE_SIZE - TEST_FLASH_BYTE_OFFSET;

    /* Fill the check buffer */
    for(i = 0; i < TEST_FLASH_BUF_LEN; i++)
    {
        checkBuf[i] = gFlashTestTxBuf[(checkIndex+i)%TEST_FLASH_PAGE_SIZE];
    }

    /* Read the  bytes from the offset across the block */
    memset(recvBuf, 0, TEST_FLASH_TEMP_BUF_SIZE);
    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, recvBuf, TEST_FLASH_BUF_LEN);

    retVal = memcmp(checkBuf, recvBuf, TEST_FLASH_BUF_LEN);
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    /*
     * CHECK 5 : Read odd bytes > 1024 from unaligned source address bytes so that DMA is used for read
     */

    /* Set an unaligned offset */
    offset = TEST_FLASH_OFFSET_BASE + TEST_FLASH_BYTE_OFFSET;

    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, gFlashTestRxBuf, TEST_FLASH_BUF_LEN_DMA);

    for(i = 0; i < TEST_FLASH_BUF_LEN_DMA; i++)
    {
        if(gFlashTestTxBuf[(i+TEST_FLASH_BYTE_OFFSET)%256] != gFlashTestRxBuf[i])
        {
            retVal = SystemP_FAILURE;
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Board_driversClose();
}

static void test_nand_flash_skip_bad_block(void *args)
{
    int32_t retVal = SystemP_SUCCESS;
    uint32_t blk, offset1, offset2, page;

    uint32_t uOffset = 0;
    uint16_t uValue;

    /* Fill the gFlashTestTxBuf with values 0-255 */
    while(uOffset < TEST_FLASH_BLOCK_SIZE)
    {
        for(uValue = 0; uValue < TEST_FLASH_DATA_MAX_VALUE; uValue++)
        {
            gFlashTestTxBuf[uOffset ++] = uValue;
        }
    }

    /* Open Flash drivers with OSPI instance as input */
    retVal = Board_driversOpen();
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Flash_Attrs *flashAttrs = Flash_getAttrs(CONFIG_FLASH0);
    Flash_Config *config = (Flash_Config*)(gFlashHandle[CONFIG_FLASH0]);
    Flash_NandOspiObject *obj = (Flash_NandOspiObject*)(config->object);

    offset1 = TEST_FLASH_OFFSET_BASE;
    offset2 = TEST_FLASH_OFFSET_BASE + 2*(flashAttrs->blockSize);

    /* Replace the spare area data with bad spare area data */
    uint32_t *goodData = obj->spareAreaData;
    obj->spareAreaData = obj->badSpareAreaData;

    memset(gFlashTestRxBuf, '\0', TEST_FLASH_PAGE_SIZE);

    /* Write bad spare area data at offset 1 */
    Flash_offsetToBlkPage(gFlashHandle[CONFIG_FLASH0], offset1, &blk, &page);
    Flash_eraseBlk(gFlashHandle[CONFIG_FLASH0], blk);
    Flash_write(gFlashHandle[CONFIG_FLASH0], offset1, gFlashTestTxBuf, TEST_FLASH_PAGE_SIZE);

    /* Write bad spare area data at offset 2 */
    Flash_offsetToBlkPage(gFlashHandle[CONFIG_FLASH0], offset2, &blk, &page);
    Flash_eraseBlk(gFlashHandle[CONFIG_FLASH0], blk);
    Flash_write(gFlashHandle[CONFIG_FLASH0], offset2, gFlashTestTxBuf, TEST_FLASH_PAGE_SIZE);

    Board_driversClose();

    /* Replace bad spare area data with good spare area data */
    obj->spareAreaData = goodData;

    /* Enable bad block check */
    obj->badBlockCheck = TRUE;

    retVal = Board_driversOpen();
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    memset(gFlashTestRxBuf, '\0', TEST_FLASH_PAGE_SIZE);

    /* Perform read write on the bad blocks */
    Flash_offsetToBlkPage(gFlashHandle[CONFIG_FLASH0], offset1, &blk, &page);
    Flash_eraseBlk(gFlashHandle[CONFIG_FLASH0], blk);
    Flash_write(gFlashHandle[CONFIG_FLASH0], offset1+(flashAttrs->pageSize*16), gFlashTestTxBuf, TEST_FLASH_BLOCK_SIZE);
    Flash_read(gFlashHandle[CONFIG_FLASH0], offset1+(flashAttrs->pageSize*16), gFlashTestRxBuf, TEST_FLASH_BLOCK_SIZE);

    retVal |= memcmp(gFlashTestTxBuf, gFlashTestRxBuf, TEST_FLASH_PAGE_SIZE);
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Board_driversClose();

    /* Disable bad block check */
    obj->badBlockCheck = FALSE;

    retVal = Board_driversOpen();
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    /* Erase bad blocks to clear bad spare area data */
    Flash_offsetToBlkPage(gFlashHandle[CONFIG_FLASH0], offset1, &blk, &page);
    Flash_eraseBlk(gFlashHandle[CONFIG_FLASH0], blk);

    Flash_offsetToBlkPage(gFlashHandle[CONFIG_FLASH0], offset2, &blk, &page);
    Flash_eraseBlk(gFlashHandle[CONFIG_FLASH0], blk);

    Board_driversClose();
}
#else
static void test_flash_readwrite(void *args)
{
    int32_t retVal = SystemP_SUCCESS;
    uint32_t blk, offset, page;

    /* Open Flash drivers with OSPI instance as input */
    retVal = Board_driversOpen();
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Flash_Attrs *flashAttrs = Flash_getAttrs(CONFIG_FLASH0);

    uint32_t loopCnt = 10U;
    uint32_t i;

    /* Set an invalid block count */
    uint32_t currBlock = flashAttrs->blockCount + 1;

    for(i=0; i<loopCnt; i++)
    {
        memset(gFlashTestRxBuf, '\0', TEST_FLASH_DATA_SIZE);
        offset = TEST_FLASH_OFFSET_BASE + i*(flashAttrs->pageSize*flashAttrs->pageCount);
        Flash_offsetToBlkPage(gFlashHandle[CONFIG_FLASH0], offset, &blk, &page);
        if(currBlock != blk)
        {
            Flash_eraseBlk(gFlashHandle[CONFIG_FLASH0], blk);
            currBlock = blk;
        }
        Flash_write(gFlashHandle[CONFIG_FLASH0], offset, gFlashTestTxBuf, TEST_FLASH_DATA_SIZE);
        Flash_read(gFlashHandle[CONFIG_FLASH0], offset, gFlashTestRxBuf, TEST_FLASH_DATA_SIZE);
        retVal |= memcmp(gFlashTestTxBuf, gFlashTestRxBuf, TEST_FLASH_DATA_SIZE);
    }
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Board_driversClose();
}

static void test_flash_read_multiple(void *args)
{
    int32_t retVal = SystemP_SUCCESS;
    uint32_t blk, offset, page;

    /* Open Flash drivers with OSPI instance as input */
    retVal = Board_driversOpen();
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Flash_Attrs *flashAttrs = Flash_getAttrs(CONFIG_FLASH0);

    /* Write two blocks of data (512 KB)*/
    uint32_t loopCnt = 2*1024;
    uint32_t i;

    memset(gFlashTestRxBuf, '\0', TEST_FLASH_DATA_SIZE);

    /* Set an invalid block count */
    uint32_t currBlock = flashAttrs->blockCount + 1;

    for(i=0; i<loopCnt; i++)
    {
        offset = TEST_FLASH_OFFSET_BASE + i*(flashAttrs->pageSize);
        Flash_offsetToBlkPage(gFlashHandle[CONFIG_FLASH0], offset, &blk, &page);
        if(currBlock != blk)
        {
            Flash_eraseBlk(gFlashHandle[CONFIG_FLASH0], blk);
            currBlock = blk;
        }
        Flash_write(gFlashHandle[CONFIG_FLASH0], offset, gFlashTestTxBuf, TEST_FLASH_DATA_SIZE);
    }

    /* READ CHECKS */

    uint8_t recvBuf[TEST_FLASH_TEMP_BUF_SIZE] __attribute__((aligned(128U)));
    uint8_t checkBuf[TEST_FLASH_TEMP_BUF_SIZE];
    uint32_t checkIndex = 0U;

    /*
     * CHECK 1 : Read odd bytes from an aligned offset
     */
    /* Set an aligned offset */
    offset = TEST_FLASH_OFFSET_BASE + 256;

    /* Fill the check buffer */
    for(i = 0; i < TEST_FLASH_BUF_LEN_ODD; i++)
    {
        checkBuf[i] = gFlashTestTxBuf[(offset+i)%TEST_FLASH_DATA_SIZE];
    }

    /* Read odd bytes from the unaligned offset */
    memset(recvBuf, 0, TEST_FLASH_TEMP_BUF_SIZE);
    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, recvBuf, TEST_FLASH_BUF_LEN_ODD);

    retVal = memcmp(checkBuf, recvBuf, TEST_FLASH_BUF_LEN_ODD);
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    /*
     * CHECK 2 : Read odd bytes from unaligned offset
     */
    /* Set an unaligned offset */
    offset = TEST_FLASH_OFFSET_BASE + 100;

    /* Fill the check buffer */
    for(i = 0; i < TEST_FLASH_BUF_LEN_ODD; i++)
    {
        checkBuf[i] = gFlashTestTxBuf[(offset+i)%TEST_FLASH_DATA_SIZE];
    }

    /* Read odd bytes from the unaligned offset */
    memset(recvBuf, 0, TEST_FLASH_TEMP_BUF_SIZE);
    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, recvBuf, TEST_FLASH_BUF_LEN_ODD);

    retVal = memcmp(checkBuf, recvBuf, TEST_FLASH_BUF_LEN_ODD);
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    /*
     * CHECK 3: Read across page boundary
     */
    /* Set offset back some odd bytes */
    offset = TEST_FLASH_OFFSET_BASE + 256 - TEST_FLASH_BYTE_OFFSET_ODD;

    /* Expected index in check buffer*/
    checkIndex = TEST_FLASH_DATA_SIZE - TEST_FLASH_BYTE_OFFSET_ODD;

    /* Fill the check buffer */
    for(i = 0; i < TEST_FLASH_BUF_LEN_ODD; i++)
    {
        checkBuf[i] = gFlashTestTxBuf[(checkIndex+i)%TEST_FLASH_DATA_SIZE];
    }

    /* Read odd bytes from the offset across the pages */
    memset(recvBuf, 0, TEST_FLASH_TEMP_BUF_SIZE);
    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, recvBuf, TEST_FLASH_BUF_LEN_ODD);

    retVal = memcmp(checkBuf, recvBuf, TEST_FLASH_BUF_LEN_ODD);
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    /*
     * CHECK 4 : Read across block boundary
     */
    /* Set offset back some odd bytes */
    offset = TEST_FLASH_OFFSET_BASE + flashAttrs->pageCount*flashAttrs->pageSize - TEST_FLASH_BYTE_OFFSET_ODD;

    /* Expected index in test buffer*/
    checkIndex = TEST_FLASH_DATA_SIZE - TEST_FLASH_BYTE_OFFSET_ODD;

    /* Fill the check buffer */
    for(i = 0; i < TEST_FLASH_BUF_LEN_ODD; i++)
    {
        checkBuf[i] = gFlashTestTxBuf[(checkIndex+i)%TEST_FLASH_DATA_SIZE];
    }

    /* Read odd bytes from the offset across the block */
    memset(recvBuf, 0, TEST_FLASH_TEMP_BUF_SIZE);
    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, recvBuf, TEST_FLASH_BUF_LEN_ODD);

    retVal = memcmp(checkBuf, recvBuf, TEST_FLASH_BUF_LEN_ODD);
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    /*
     * CHECK 5 : Read odd bytes > 1024 from unaligned source address bytes so that DMA is used for read
     */

    /* Set an unaligned offset */
    offset = TEST_FLASH_OFFSET_BASE + TEST_FLASH_BYTE_OFFSET_ODD;

    Flash_read(gFlashHandle[CONFIG_FLASH0], offset, gFlashTestRxBuf, TEST_FLASH_BUF_LEN_ODD_DMA);

    for(i = 0; i < TEST_FLASH_BUF_LEN_ODD_DMA; i++)
    {
        if(gFlashTestTxBuf[(i+TEST_FLASH_BYTE_OFFSET_ODD)%256] != gFlashTestRxBuf[i])
        {
            retVal = SystemP_FAILURE;
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT32(SystemP_SUCCESS, retVal);

    Board_driversClose();
}
#endif
