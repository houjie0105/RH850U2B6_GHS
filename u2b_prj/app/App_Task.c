/**************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
***************************************************************************/

/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
#include "main.h"

#include "r_clk.h"
#include "r_port.h"
#include "r_ostm.h"
#include "r_stbc.h"
#include "r_intc.h"
#include "r_br.h"
#include "r_rscfd.h"
#include "r_i2c.h"

/*****************************************************************************
 Macro definitions
 *****************************************************************************/
//#define CAN_TEST

/*****************************************************************************
 Typedef definitions
 *****************************************************************************/

/*****************************************************************************
 Global variables
 *****************************************************************************/

/*****************************************************************************
 Global static variables
 *****************************************************************************/
volatile uint8 Gu1PerInitFinished = 1UL;
extern uint32 Gu4Task_100MsFlag;



#ifdef CAN_TEST
#define RSCFD_CAN_TEST
#define RSCFD_TRFIFO_TEST
#define DIM(a) (sizeof(a)/sizeof((a)[0]))

static uint8 Gsu1aTxDataBuff[64] = {
    0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U, 0x09U, 0x0AU, 0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x0FU,
    0x10U, 0x11U, 0x12U, 0x13U, 0x14U, 0x15U, 0x16U, 0x17U, 0x18U, 0x19U, 0x1AU, 0x1BU, 0x1CU, 0x1DU, 0x1EU, 0x1FU,
    0x20U, 0x21U, 0x22U, 0x23U, 0x24U, 0x25U, 0x26U, 0x27U, 0x28U, 0x29U, 0x2AU, 0x2BU, 0x2CU, 0x2DU, 0x2EU, 0x2FU,
    0x30U, 0x31U, 0x32U, 0x33U, 0x34U, 0x35U, 0x36U, 0x37U, 0x38U, 0x39U, 0x3AU, 0x3BU, 0x3CU, 0x3DU, 0x3EU, 0x3FU,
};

static const uint8 Gscu1aRxRuleSize[8] = {
    3U, 3U, 3U, 0U, 0U, 3U, 0U, 0U,
};

/* CFDGAFLID */
/* CFDGAFLM */
/* CFDGAFLP0_ */
/* CFDGAFLP1_ */
static const uint32 Gscu4aaCanRecvRule[][4] = {
    {
        GAFLID(0x604UL), 
        GAFLIDM(0x7FFUL),
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 8U)
        #else
        0x00000000UL,
        GAFLFDPD(1UL << 0U)
        #endif
    },
    {
        GAFLID(0x703UL), 
        GAFLIDM(0x7FFUL), 
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 9U)
        #else
        0x00000000UL,
        GAFLFDPD(1UL << 1U)
        #endif
    },
    {
        GAFLID(0x7DFUL), 
        GAFLIDM(0x7FFUL), 
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 10U)
        #else
        0x00000000UL, 
        GAFLFDPD(1UL << 2U)
        #endif
    },
    {
        GAFLID(0x604UL), 
        GAFLIDM(0x7FFUL),
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 11U)
        #else
        0x00000000UL,
        GAFLFDPD(1UL << 0U)
        #endif
    },
    {
        GAFLID(0x703UL), 
        GAFLIDM(0x7FFUL), 
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 12U)
        #else
        0x00000000UL,
        GAFLFDPD(1UL << 1U)
        #endif
    },
    {
        GAFLID(0x7DFUL), 
        GAFLIDM(0x7FFUL), 
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 13U)
        #else
        0x00000000UL, 
        GAFLFDPD(1UL << 2U)
        #endif
    },
    {
        GAFLID(0x604UL), 
        GAFLIDM(0x7FFUL),
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 14U)
        #else
        0x00000000UL,
        GAFLFDPD(1UL << 0U)
        #endif
    },
    {
        GAFLID(0x703UL), 
        GAFLIDM(0x7FFUL), 
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 15U)
        #else
        0x00000000UL,
        GAFLFDPD(1UL << 1U)
        #endif
    },
    {
        GAFLID(0x7DFUL), 
        GAFLIDM(0x7FFUL), 
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 16U)
        #else
        0x00000000UL, 
        GAFLFDPD(1UL << 2U)
        #endif
    },
    {
        GAFLID(0x604UL), 
        GAFLIDM(0x7FFUL),
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 23U)
        #else
        0x00000000UL,
        GAFLFDPD(1UL << 0U)
        #endif
    },
    {
        GAFLID(0x703UL), 
        GAFLIDM(0x7FFUL), 
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 24U)
        #else
        0x00000000UL,
        GAFLFDPD(1UL << 1U)
        #endif
    },
    {
        GAFLID(0x7DFUL), 
        GAFLIDM(0x7FFUL), 
        #ifdef RSCFD_TRFIFO_TEST
        0x00000000UL,
        GAFLFDPD(1UL << 25U)
        #else
        0x00000000UL, 
        GAFLFDPD(1UL << 2U)
        #endif
    },
};

/* CFDRFCC */
static const uint32 Gscu4aRxFifoCfg[] = {
    #ifdef RSCFD_TRFIFO_TEST
    0x00000000UL,
    #else
    RFIM | RFDC(1UL) | RFPLS(7UL) | RFIE | RFE,
    RFIM | RFDC(1UL) | RFPLS(7UL) | RFIE | RFE,
    RFIM | RFDC(1UL) | RFPLS(7UL) | RFIE | RFE,
    #endif
};

/* CFDCFCC */
static const uint32 Gscu4aRxTxFifoCfg[] = {
    #ifndef RSCFD_TRFIFO_TEST
    0x00000000UL,
    #else
    //ch 0
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    //ch 1
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    //ch 2
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    //ch 3
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    //ch 4
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    //ch 5
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    CFIM | CFDC(1UL) | CFPLS(7UL) | CFRXIE | CFE,
    #endif
};

static const CanGlobalCfg GscstCanGlobal = {
    RSCFD_TSRST,
    RSCFD_ITRCP(1UL) | RSCFD_TSP(0UL),
    RSCFD_TSCCFG(0UL),
    RSCFD_RMPLS(0UL) | (RSCFD_NRXMB(2UL)),
    (uint8 *)Gscu1aRxRuleSize,
    (uint32 (*)[4])Gscu4aaCanRecvRule,
    DIM(Gscu4aRxFifoCfg), 
    (uint32 *)Gscu4aRxFifoCfg,
    DIM(Gscu4aRxTxFifoCfg),
    (uint32 *)Gscu4aRxTxFifoCfg,
};

static const Rscfd_CanCfg GscstCanChCfg0 = {
    (CanGlobalCfg *)(&GscstCanGlobal),
    RSCFD_BOM(0UL),
    0x00U,
    0x0a180007UL,//RSCFD_NTSEG2(5UL) | RSCFD_NTSEG1(12UL) | RSCFD_NSJW(0UL) | RSCFD_NBRP(3UL),
    0x00010107UL,//RSCFD_DSJW(0UL) | RSCFD_DTSEG2(1UL) | RSCFD_DTSEG1(1UL) | RSCFD_DBRP(3UL),
};

static const Rscfd_TxBuffData GscstSendBuff[] = {
    {
        0U,
        0U,
        RSCFD_TMID(0x605UL),
        #ifdef RSCFD_CAN_TEST
        RSCFD_TMDLC(0x08UL),
        0x00U,
        #else
        RSCFD_TMDLC(0x0FUL),
        RSCFD_TMFDF | RSCFD_TMBRS,
        #endif
        RSCFD_TMTR,
        (uint32*)(Gsu1aTxDataBuff),
    },
    {
        0U,
        0U,
        RSCFD_TMID(0x7B3UL),
        #ifdef RSCFD_CAN_TEST
        RSCFD_TMDLC(0x08UL),
        0x00U,
        #else
        RSCFD_TMDLC(0x0FUL),
        RSCFD_TMFDF | RSCFD_TMBRS,
        #endif
        RSCFD_TMTR,
        (uint32*)(Gsu1aTxDataBuff),
    },
};
#endif
/*****************************************************************************
 Static functions
 *****************************************************************************/

/*****************************************************************************
--------------------- Global functions --------------------------------------
 *****************************************************************************/

/*****************************************************************************
 * Declaration : void Main_Init(void)
 * Description : User initialization program entry
 * Parameters  : None
 * Return      : None
 *****************************************************************************/
void Main_Init(void)
{
    R_BR_SetChk(0U);
    while (!R_BR_GetSync(0U));
    uint32 Lu4Peid;
    Lu4Peid = GetPEID();
    if (PE_CPU0 == (Lu4Peid & 0x1FUL))
    {
        R_CLK_Init();

        R_STBC_CfgMsr(MSR_OSTM, 0x00UL);
        R_STBC_CfgMsr(MSR_RSCFD, 0x00UL);

        R_PORT_EnablePort();
        //R_PORT_Init(0, 0, 0x00);

        /* RSCAN Ch1 */
        R_PORT_Init(0U, 0U, PORT_PMC | PORT_APF7);
        R_PORT_Init(0U, 1U, PORT_PMC | PORT_APF12 | PORT_PM);

        /* RSCAN Ch0 */
        R_PORT_Init(2U, 2U, PORT_PMC | PORT_APF7 | PORT_PM);
        R_PORT_Init(2U, 0U, PORT_PMC | PORT_APF7);

        /* RSCAN Ch2 */
        R_PORT_Init(10U, 7U, PORT_PMC | PORT_APF7);
        R_PORT_Init(10U, 4U, PORT_PMC | PORT_APF7 | PORT_PM);

        #ifdef CAN_TEST
        #ifndef RSCFD_TRFIFO_TEST
        R_INTC_Init(653UL, INTC_EITB & INTC_EIMK | INTC_EIP(6UL));
        R_INTC_Bind(653UL, INTC_PEID(0UL) | INTC_GPID(0UL));
        #else
        R_INTC_Init(665UL, INTC_EITB & INTC_EIMK | INTC_EIP(6UL));
        R_INTC_Bind(665UL, INTC_PEID(0UL) | INTC_GPID(0UL));
        R_INTC_Init(659UL, INTC_EITB & INTC_EIMK | INTC_EIP(6UL));
        R_INTC_Bind(659UL, INTC_PEID(0UL) | INTC_GPID(0UL));
        R_INTC_Init(671UL, INTC_EITB & INTC_EIMK | INTC_EIP(6UL));
        R_INTC_Bind(671UL, INTC_PEID(0UL) | INTC_GPID(0UL));
        #endif
        R_RSCFD_Init(0U, 0U, GscstCanChCfg0);
        R_RSCFD_Init(0U, 1U, GscstCanChCfg0);
        R_RSCFD_Init(0U, 2U, GscstCanChCfg0);
        R_RSCFD_Start(0U, 0U, GscstCanChCfg0);
        R_RSCFD_Start(0U, 1U, GscstCanChCfg0);
        R_RSCFD_Start(0U, 2U, GscstCanChCfg0);
        #endif

        R_INTC_Init(360UL, INTC_EITB & INTC_EIMK | INTC_EIP(6UL));
        R_OSTM_InitCtl(0U, OSTM_IE);
        R_OSTM_SetCmp(0U, 7999999UL);
        R_OSTM_Start(0U);


        R_I2C_Init();
        {
            static uint8 Lu1TxBuff[4] = {0};
            static uint8 Lu1RxBuff[4] = {0};
            uint16 Lu1I2CRet = E_OK;
            uint8 Lu1I2CSt = I2C_ST_IDLE;
            uint32 Lu4I2CTimout = 0xFFFF;
            Lu1TxBuff[0] = 0x60;
            Lu1I2CRet = R_I2C_WriteRead(0x18,Lu1TxBuff,1,Lu1RxBuff,1);
            do
            {
                Lu1I2CSt = R_I2C_GetDevStatus();
                Lu4I2CTimout --;
            }while((Lu1I2CSt==I2C_ST_BUSY)&&Lu4I2CTimout);

        }

        Gu1PerInitFinished = 0UL;
    }
    else  if (PE_CPU1 == (Lu4Peid & 0x1FUL))
    {
        while (Gu1PerInitFinished);
    }
    else
    {
        while (Gu1PerInitFinished);
    }
    ENABLE_INTERRUPT();
}

#ifdef CAN_TEST
Can_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType *PduInfo)
{
    uint8 Lu1Loop;
    for (Lu1Loop = 0U; Lu1Loop < 64U; Lu1Loop++)
    {
        ((uint8*)Gsu1aTxDataBuff)[Lu1Loop] = PduInfo->sdu[Lu1Loop];
    }
    return (Can_ReturnType)R_RSCFD_BuffSend(0U, 0U, GscstSendBuff[Hth]);
}
#endif

/*****************************************************************************
 * Declaration : void Main_Idle_Task(void)
 * Description : Idle task
 * Parameters  : None
 * Return      : None 
 *****************************************************************************/
void Main_Idle_Task(void)
{
    uint32 Lu4Peid;
    Lu4Peid = GetPEID();
    if (PE_CPU0 == (Lu4Peid & 0x1FUL))
    {
        if(Gu4Task_100MsFlag)
        {
            static uint16 GuCount1S;
            Gu4Task_100MsFlag = 0;
            if(GuCount1S >= 10)
            {
                GuCount1S = 0;
                R_PORT_FlipPin(0, 0);
            }
            GuCount1S++;
            #ifdef CAN_TEST
            {
                static const uint8 Lscu1aBuff[64] = {
                    0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                    0x10U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                    0x20U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                    0x30U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                    0x40U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                    0x50U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                    0x60U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                    0x70U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                };
                static Can_PduType LstCanPduType;
                LstCanPduType.length = 0x0FU; /* This value does not affect sending */
                LstCanPduType.swPduHandle = 0x02U; /* This value does not affect sending */
                LstCanPduType.id = 0x00UL; /* This value does not affect sending */
                LstCanPduType.sdu = (uint8 *)Lscu1aBuff;
                Can_Write(0U, &LstCanPduType);
            }
            #endif
        }
    }
    else if (PE_CPU1 == (Lu4Peid & 0x1FUL))
    {
    }
    else
    {
    }
}

/*****************************************************************************
--------------------- Static functions --------------------------------------
 *****************************************************************************/
