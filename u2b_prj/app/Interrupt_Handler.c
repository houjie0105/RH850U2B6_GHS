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
#include "r_compiler.h"
#include "r_rscfd.h"
#include "r_i2c.h"

/*****************************************************************************
 Macro definitions
 *****************************************************************************/
/* 64 is databyte length */
#define DIM2(a) (sizeof(a)/64U)
static uint32 Gsu4aaRxDataFIFO[][16] = {
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
};
static Rscfd_RxFifoData GsstRxFIFO[] = {
    {
        0U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRxDataFIFO[0][0]),
    },
    {
        1U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRxDataFIFO[1][0]),
    },
    {
        2U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRxDataFIFO[2][0]),
    },
    {
        3U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRxDataFIFO[3][0]),
    },
    {
        4U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRxDataFIFO[4][0]),
    },
    {
        5U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRxDataFIFO[5][0]),
    },
};

static uint32 Gsu4aaRTFifoDataFIFO[][16] = {
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
    { 0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL, \
      0x00010203UL, 0x04050607UL, 0x00010203UL, 0x04050607UL},
};
static Rscfd_RxTxFifoData GsstTxRxFIFO[] = {
    {
        0U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRTFifoDataFIFO[0][0]),
    },
    {
        1U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRTFifoDataFIFO[1][0]),
    },
    {
        2U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRTFifoDataFIFO[2][0]),
    },
    {
        3U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRTFifoDataFIFO[3][0]),
    },
    {
        4U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRTFifoDataFIFO[4][0]),
    },
    {
        5U, 
        0U,
        0U,
        0U,
        (uint32*)(&Gsu4aaRTFifoDataFIFO[5][0]),
    },
};
/*****************************************************************************
 Typedef definitions
 *****************************************************************************/

/*****************************************************************************
 Global variables
 *****************************************************************************/
uint32 Gu4Task_100MsFlag = 0;
/*****************************************************************************
 Global static variables
 *****************************************************************************/
/*****************************************************************************
 Static functions
 *****************************************************************************/

/*****************************************************************************
--------------------- Global functions --------------------------------------
 *****************************************************************************/
/*****************************************************************************
 * Declaration : 
 * Description : 
 * Parameters  : 
 * Return      :  
 *****************************************************************************/
/*****************************************************************************
--------------------- Static functions --------------------------------------
 *****************************************************************************/

__interrupt void Ostm_O0_100ms_Handler(void)
{
    Gu4Task_100MsFlag = 1;
}

__interrupt void Ostm_O1_100ms_Handler(void)
{
    R_I2C_MainFunction();
}

void CanIf_TxRxIndication(const Can_HwType *Mailbox, const PduInfoType *PduInfoPtr)
{

}

void CanIf_RxIndication(const Can_HwType *Mailbox, const PduInfoType *PduInfoPtr)
{

}

__interrupt void CAN_ReceiveFIFO_Interrupt(void)
{
    uint8 Lu1HrhIdx;
    PduInfoType LstPduInfo;
    Can_HwType LstMailbox;
    for (Lu1HrhIdx = 0U; Lu1HrhIdx < DIM2(Gsu4aaRxDataFIFO); Lu1HrhIdx++)
    {
        if (R_OK == R_RSCFD_ReadRxFifo(0U, 0U, &GsstRxFIFO[Lu1HrhIdx]))
        {
            LstMailbox.Hoh = Lu1HrhIdx;
            LstMailbox.ControllerId = 0U;
            LstMailbox.CanId = GsstRxFIFO[Lu1HrhIdx].u4CFDRFID;
            LstPduInfo.SduDataPtr = (uint8*)GsstRxFIFO[Lu1HrhIdx].u4pCFDRFDF;
            LstPduInfo.SduLength = GsstRxFIFO[Lu1HrhIdx].u4CFDRFPTR >> 28U;
            CanIf_RxIndication(&LstMailbox, &LstPduInfo);
        }
    }
}
/*
    uint32 u4RxTxFifoId;
    uint32 u4CFDCFID;
    uint32 u4CFDCFPTR;
    uint32 u4CFDCFFDCSTS;
    uint32 *u4pCFDCFDF;
*/

__interrupt void CAN_ReceiveTRFIFO0_Interrupt(void)
{
    uint8 Lu1HrhIdx;
    PduInfoType LstPduInfo;
    Can_HwType LstMailbox;
    for (Lu1HrhIdx = 0U; Lu1HrhIdx < DIM2(Gsu4aaRTFifoDataFIFO); Lu1HrhIdx++)
    {
        if (R_OK == R_RSCFD_ReadRxTxFifo(0U, 0U, &GsstTxRxFIFO[Lu1HrhIdx]))
        {
            LstMailbox.Hoh = Lu1HrhIdx;
            LstMailbox.ControllerId = 0U;
            LstMailbox.CanId = GsstTxRxFIFO[Lu1HrhIdx].u4CFDCFID;
            LstPduInfo.SduDataPtr = (uint8*)GsstTxRxFIFO[Lu1HrhIdx].u4pCFDCFDF;
            LstPduInfo.SduLength = GsstTxRxFIFO[Lu1HrhIdx].u4CFDCFPTR >> 28U;
            CanIf_TxRxIndication(&LstMailbox, &LstPduInfo);
        }
    }
}
__interrupt void CAN_ReceiveTRFIFO1_Interrupt(void)
{
    uint8 Lu1HrhIdx;
    PduInfoType LstPduInfo;
    Can_HwType LstMailbox;
    for (Lu1HrhIdx = 0U; Lu1HrhIdx < DIM2(Gsu4aaRTFifoDataFIFO); Lu1HrhIdx++)
    {
        if (R_OK == R_RSCFD_ReadRxTxFifo(0U, 1U, &GsstTxRxFIFO[Lu1HrhIdx]))
        {
            LstMailbox.Hoh = Lu1HrhIdx;
            LstMailbox.ControllerId = 0U;
            LstMailbox.CanId = GsstTxRxFIFO[Lu1HrhIdx].u4CFDCFID;
            LstPduInfo.SduDataPtr = (uint8*)GsstTxRxFIFO[Lu1HrhIdx].u4pCFDCFDF;
            LstPduInfo.SduLength = GsstTxRxFIFO[Lu1HrhIdx].u4CFDCFPTR >> 28U;
            CanIf_TxRxIndication(&LstMailbox, &LstPduInfo);
        }
    }
}

__interrupt void CAN_ReceiveTRFIFO2_Interrupt(void)
{
    uint8 Lu1HrhIdx;
    PduInfoType LstPduInfo;
    Can_HwType LstMailbox;
    for (Lu1HrhIdx = 0U; Lu1HrhIdx < DIM2(Gsu4aaRTFifoDataFIFO); Lu1HrhIdx++)
    {
        if (R_OK == R_RSCFD_ReadRxTxFifo(0U, 2U, &GsstTxRxFIFO[Lu1HrhIdx]))
        {
            LstMailbox.Hoh = Lu1HrhIdx;
            LstMailbox.ControllerId = 0U;
            LstMailbox.CanId = GsstTxRxFIFO[Lu1HrhIdx].u4CFDCFID;
            LstPduInfo.SduDataPtr = (uint8*)GsstTxRxFIFO[Lu1HrhIdx].u4pCFDCFDF;
            LstPduInfo.SduLength = GsstTxRxFIFO[Lu1HrhIdx].u4CFDCFPTR >> 28U;
            CanIf_TxRxIndication(&LstMailbox, &LstPduInfo);
        }
    }
}

__interrupt void CAN_ReceiveTRFIFO5_Interrupt(void)
{
    uint8 Lu1HrhIdx;
    PduInfoType LstPduInfo;
    Can_HwType LstMailbox;
    for (Lu1HrhIdx = 0U; Lu1HrhIdx < DIM2(Gsu4aaRTFifoDataFIFO); Lu1HrhIdx++)
    {
        if (R_OK == R_RSCFD_ReadRxTxFifo(0U, 5U, &GsstTxRxFIFO[Lu1HrhIdx]))
        {
            LstMailbox.Hoh = Lu1HrhIdx;
            LstMailbox.ControllerId = 0U;
            LstMailbox.CanId = GsstTxRxFIFO[Lu1HrhIdx].u4CFDCFID;
            LstPduInfo.SduDataPtr = (uint8*)GsstTxRxFIFO[Lu1HrhIdx].u4pCFDCFDF;
            LstPduInfo.SduLength = GsstTxRxFIFO[Lu1HrhIdx].u4CFDCFPTR >> 28U;
            CanIf_TxRxIndication(&LstMailbox, &LstPduInfo);
        }
    }
}
