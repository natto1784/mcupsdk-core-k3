/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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

#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/HwiP.h>
#include <drivers/epwm.h>
#include <drivers/eqep.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

/*
 * This example uses the EQEP module to measure frequency of a signal
 * generated by ePWM module
 *
 * In this example EQEP0 is used to measure frequency, the user can also
 * select a different one.
 *
 * This example also showcases how to configure and use the EQEP module.
 */

/* Defines used by example */
/* Sysclk frequency */
#define DEVICE_SYSCLK_FREQ  (200000000U)
/* We want to output at 5 kHz */
#define PWM_CLK  (5000U)
/* Base/max frequency is 10 kHz */
#define BASE_FREQ  (10000U)

#define FREQ_SCALER_PR  (((DEVICE_SYSCLK_FREQ / 128) * 8) / (2 * BASE_FREQ))

#define FREQ_SCALER_FR  ((BASE_FREQ * 2) / 100)
/* App Run Time in seconds */
#define APP_RUN_TIME  (10U)
/* Macro for interrupt pulse */
#define APP_INT_IS_PULSE  (1U)

/* Global variables and objects */
uint32_t gEpwmBaseAddr;
uint32_t gEqepBaseAddr;
static HwiP_Object gEpwmHwiObject;
static SemaphoreP_Object gEpwmSyncSemObject;
typedef struct
{
    uint32_t freqScalerPR;  /* Parameter: Scaler converting 1/N cycles to a */
                            /* GLOBAL_Q freq (Q0) - independently with global Q */
    uint32_t freqScalerFR;  /* Parameter: Scaler converting 1/N cycles to a */
                            /* GLOBAL_Q freq (Q0) - independently with global Q */
    uint32_t baseFreq;      /* Parameter: Maximum freq */

    Float32 freqPR;         /* Output: Freq in per-unit using capture unit */
    int32_t freqHzPR;       /* Output: Freq in Hz, measured using Capture unit */
    uint32_t oldPos;

    Float32 freqFR;         /* Output: Freq in per-unit using position counter */
    int32_t freqHzFR;       /* Output: Freq in Hz, measured using Capture unit */
} FreqCal_Object;
typedef FreqCal_Object *FreqCal_Handle;
FreqCal_Object freq =
{
    FREQ_SCALER_PR,  /* freqScalerPR */
    FREQ_SCALER_FR,  /* freqScalerFR */
    BASE_FREQ,       /* baseFreq */
    0, 0, 0, 0, 0    /* Initialize outputs to zero */
};
uint32_t count = 0;  /* counter to check measurement gets saturated */
uint32_t pass=0, fail =0; /* Pass or fail indicator */

/* Function prototypes */
static void App_epwmIntrISR(void *handle);
static void FreqCal_calculate(FreqCal_Handle, uint32_t*);

void eqep_frequency_measurement_main(void *args)
{
    int32_t status;
    uint32_t numIsrCnt = (APP_RUN_TIME * PWM_CLK);
    HwiP_Params hwiPrms;

    /* Open drivers to open the UART driver for console */
    Drivers_open();
    Board_driversOpen();

    DebugP_log("EQEP Frequency Measurement Test Started ...\r\n");

    gEpwmBaseAddr = CONFIG_EPWM0_BASE_ADDR;
    gEqepBaseAddr = CONFIG_EQEP0_BASE_ADDR;

    status = SemaphoreP_constructCounting(&gEpwmSyncSemObject, 0, numIsrCnt);
    DebugP_assert(SystemP_SUCCESS == status);

    /* Register & enable interrupt */
    HwiP_Params_init(&hwiPrms);
    /* Integrate with Syscfg */
    hwiPrms.intNum      = CSLR_R5FSS0_CORE0_CONTROLSS_INTRXBAR0_OUT_0;
    hwiPrms.callback    = &App_epwmIntrISR;
    /* Integrate with Syscfg */
    hwiPrms.isPulse     = APP_INT_IS_PULSE;
    status              = HwiP_construct(&gEpwmHwiObject, &hwiPrms);
    DebugP_assert(status == SystemP_SUCCESS);

    /* Clear ePWM interrupt */
    EPWM_clearEventTriggerInterruptFlag(gEpwmBaseAddr);

    while(numIsrCnt > 0)
    {
        SemaphoreP_pend(&gEpwmSyncSemObject, SystemP_WAIT_FOREVER);
        numIsrCnt--;
    }

    EPWM_disableInterrupt(gEpwmBaseAddr);
    EPWM_clearEventTriggerInterruptFlag(gEpwmBaseAddr);     /* Clear any pending interrupts if any */
    HwiP_destruct(&gEpwmHwiObject);
    SemaphoreP_destruct(&gEpwmSyncSemObject);

    if (pass)
    {
        DebugP_log("EQEP Frequency Measurement Test Passed!!\r\n");
        DebugP_log("All tests have passed!!\r\n");
    }
    else
    {
        DebugP_log("EQEP Frequency Measurement Test Failed!!\r\n");
    }

    Board_driversClose();
    Drivers_close();
}

static void App_epwmIntrISR(void *handle)
{
    volatile bool status;

    /* Checks for events and calculates frequency. */
    FreqCal_calculate(&freq, &count);

    /* Comparing the eQEP measured frequency with the ePWM frequency */
    /* After count becomes 3 , eQEP measurement gets saturated and */
    /* classifying pass = 1 if measured frequency is 50 more or */
    /* less than input */
    if (count >= 3)
    {
        if (((freq.freqHzFR - PWM_CLK) < 50) && ((freq.freqHzFR - PWM_CLK) > -50))
        {
            pass = 1; fail = 0;
        }
        else
        {
            fail = 1; pass = 0;
        }
    }

    status = EPWM_getEventTriggerInterruptStatus(gEpwmBaseAddr);
    if(status == true)
    {
        SemaphoreP_post(&gEpwmSyncSemObject);
        EPWM_clearEventTriggerInterruptFlag(gEpwmBaseAddr);
    }

    return;
}

static void FreqCal_calculate(FreqCal_Object *p, uint32_t *c)
{
    uint32_t temp;
    Float32 newPosCnt, oldPosCnt;

    /* Frequency calculation using eQEP position counter */
    if((EQEP_getInterruptStatus(gEqepBaseAddr) & EQEP_INT_UNIT_TIME_OUT) != 0)
    {
        /* Incrementing the count value */
        (*c)++;
        /* Get latched POSCNT value */
        newPosCnt = EQEP_getPositionLatch(gEqepBaseAddr);
        oldPosCnt = p->oldPos;

        if(newPosCnt > oldPosCnt)
        {
            /* x2 - x1 in v = (x2 - x1) / T equation */
            temp = newPosCnt - oldPosCnt;
        }
        else
        {
            temp = ((float)0xFFFFFFFF - oldPosCnt) + newPosCnt;
        }

        /* p->freqFR = (x2 - x1) / (T * 10kHz) */
        p->freqFR = ((Float32)temp / (Float32)p->freqScalerFR);
        temp=p->freqFR;

        /* Is freq greater than max freq (10kHz for this example)? */
        if(temp >= 1)
        {
            p->freqFR = 1;
        }
        else
        {
            p->freqFR = temp;
        }

        /* Q0 = Q0 * GLOBAL_Q */
        /* p->freqHzFR = (p->freqFR) * 10kHz = (x2 - x1) / T */
        p->freqHzFR = (p->baseFreq * p->freqFR);

        /* Update old position counter value and clear unit time out flag */
        p->oldPos = newPosCnt;
        EQEP_clearInterruptStatus(gEqepBaseAddr, EQEP_INT_UNIT_TIME_OUT);
    }

    /* Frequency calculation using eQEP capture counter */
    if((EQEP_getStatus(gEqepBaseAddr) & EQEP_STS_UNIT_POS_EVNT) != 0)
    {
        /* No capture overflow */
        if((EQEP_getStatus(gEqepBaseAddr) & EQEP_STS_CAP_OVRFLW_ERROR) == 0)
        {
            temp = (uint32_t)EQEP_getCapturePeriodLatch(gEqepBaseAddr);
        }
        else
        {
            /* Capture overflow, saturate the result */
            temp = 0xFFFF;
        }

        /* p->freqPR = X / [(t2 - t1) * 10kHz] */
        p->freqPR = ((Float32)p->freqScalerPR /  (Float32)temp);
        temp = p->freqPR;

        if(temp > 1)
        {
            p->freqPR = 1;
        }
        else
        {
            p->freqPR = temp;
        }

        /* Q0 = Q0 * GLOBAL_Q */
        /* p->freqHzPR = (p->freqPR) * 10kHz = X / (t2 - t1) */
        p->freqHzPR = (p->baseFreq * p->freqPR);

        /* Clear unit position event flag and overflow error flag */
        EQEP_clearStatus(gEqepBaseAddr, (EQEP_STS_UNIT_POS_EVNT |
                                      EQEP_STS_CAP_OVRFLW_ERROR));
    }
}
