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
#include "r_ostm.h"

#include "r_ostm_reg.h"

/*****************************************************************************
 Macro definitions
 *****************************************************************************/

/*****************************************************************************
 Typedef definitions
 *****************************************************************************/

/*****************************************************************************
 Global variables
 *****************************************************************************/

/*****************************************************************************
 Global static variables
 *****************************************************************************/
static const uint32 Gscu4Ostm[] = {
    OSTM0,
    OSTM1,
    OSTM2,
    OSTM3,
    OSTM4,
    OSTM5,
};

/*****************************************************************************
 Static functions
 *****************************************************************************/

/*****************************************************************************
--------------------- Global functions --------------------------------------
 *****************************************************************************/
/*****************************************************************************
 * Declaration : void R_OSTM_InitCtl(uint8 Pu1Ostm, uint8 Pu1Cfg)
 * Description : OSTM Initialize.
 * Parameters  : Pu1Ostm - OSTM
 *               Pu1Cfg  - OSTM configuration
 * Return      : None
 *****************************************************************************/
void R_OSTM_InitCtl(uint8 Pu1Ostm, uint8 Pu1Cfg)
{
    do {
        CTL(Gscu4Ostm[Pu1Ostm]) = Pu1Cfg;
    } while (Pu1Cfg != CTL(Gscu4Ostm[Pu1Ostm]));  
}

/*****************************************************************************
 * Declaration : void R_OSTM_SetCmp(uint8 Pu1Ostm, uint32 Pu4CmpVal)
 * Description : Set OSTM Compare value.
 * Parameters  : Pu1Ostm   - OSTM
 *               Pu4CmpVal - Compare value 
 * Return      : None
 *****************************************************************************/
void R_OSTM_SetCmp(uint8 Pu1Ostm, uint32 Pu4CmpVal)
{
    CMP(Gscu4Ostm[Pu1Ostm]) = Pu4CmpVal;
}

/*****************************************************************************
 * Declaration : void R_OSTM_Start(uint8 Pu1Ostm)
 * Description : Start OSTM.
 * Parameters  : Pu1Ostm - OSTM
 * Return      : None
 *****************************************************************************/
void R_OSTM_Start(uint8 Pu1Ostm)
{
    TS(Gscu4Ostm[Pu1Ostm]) = 0x01U;
}

/*****************************************************************************
 * Declaration : void R_OSTM_Stop(uint8 Pu1Ostm)
 * Description : Stop OSTM.
 * Parameters  : Pu1Ostm - OSTM
 * Return      : None
 *****************************************************************************/
void R_OSTM_Stop(uint8 Pu1Ostm)
{
    TT(Gscu4Ostm[Pu1Ostm]) = 0x01U;
}

/*****************************************************************************
--------------------- Static functions --------------------------------------
 *****************************************************************************/
