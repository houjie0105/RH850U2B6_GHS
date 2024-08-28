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
#include "r_clk.h"

#include "r_clk_reg.h"

/*****************************************************************************
 Macro definitions
 *****************************************************************************/
#define KCPROT_ENABLE          0xA5A5A501UL
#define KCPROT_DISABLE         0xA5A5A500UL

/*****************************************************************************
 Typedef definitions
 *****************************************************************************/

/*****************************************************************************
 Global variables
 *****************************************************************************/

/*****************************************************************************
 Global static variables
 *****************************************************************************/

/*****************************************************************************
 Static functions
 *****************************************************************************/
uint8 R_GetPllStatus(void)                                                     /* PRQA S 1532 # JV-01 */
{
    uint8 LddPllLockStatus = 0;
    uint8 LucCount;
    if (0x01 == (MCU_PLLS & 0x01))                                                  /* PRQA S 0303 # JV-01 */
    {
        /*Read the PLLS.PLLCLKSTAB = 1 four times.*/
        for (LucCount = 0; LucCount < 4; LucCount++)
        {
            if (0 == (MCU_PLLS & 0x02))
            {
                LddPllLockStatus = 1;
            }
        }
    }
    else
    {
        LddPllLockStatus = 1;
    }
    return (LddPllLockStatus);
}


/*****************************************************************************
--------------------- Global functions --------------------------------------
 *****************************************************************************/
/*****************************************************************************
 * Declaration : void R_CLK_Init(void)
 * Description : Clock Initialize.
 * Parameters  : None
 * Return      : None
 *****************************************************************************/
void R_CLK_Init(void)
{
    /* Main osc enable */
    MCU_CLKKCPROT1 = KCPROT_ENABLE;
    MCU_MOSCE = 0x01;
    MCU_CLKKCPROT1 = KCPROT_DISABLE;
    /* check */
    {
        uint32 LucCount = 0;
        /*Read the PLLS.PLLCLKSTAB = 1 four times.*/
        for (LucCount = 0; LucCount < 400; LucCount++)
        {
            if (MCU_MOSCS != 0x03UL)
            {
                LucCount = 0;
            }
        }
    }


    /* PLL enable */
    MCU_CLKKCPROT1 = KCPROT_ENABLE;
    MCU_PLLE = 0x01;
    MCU_CLKKCPROT1 = KCPROT_DISABLE;
    /* check */

    {
        uint32 LucIntCount = 10000UL;
        while((LucIntCount > 0UL) && (MCU_PLLS != R_GetPllStatus()))
        {
            LucIntCount--;
        }
    }
    {
        uint32 LucCount = 0;
        /*Read the PLLS.PLLCLKSTAB = 1 four times.*/
        for (LucCount = 0; LucCount < 400; LucCount++)
        {
            if (MCU_CKD_PLLS != 0x02UL)
            {
                LucCount = 0;
            }
        }
    }
    /* PLL enable */
    MCU_CLKKCPROT1 = KCPROT_ENABLE;
    MCU_CKS_CLEAN = 0x00;
    MCU_CLKKCPROT1 = KCPROT_DISABLE;
    /* check */
    {
        uint32 LucCount = 0;
        /*Read the PLLS.PLLCLKSTAB = 1 four times.*/
        for (LucCount = 0; ((LucCount < 20000) && (MCU_CKS_CLEANS != 0x00)); LucCount++)
        {
        }
    }

    {
        uint32 LucCount = 0;
        MCU_CLKKCPROT1 = KCPROT_ENABLE;
        MCU_CKD_PLLC = 0x0;
        MCU_CLKKCPROT1 = KCPROT_DISABLE;
        /* check */
        for (LucCount = 0; (LucCount < 20000) && (MCU_CKD_PLLS != 0x02UL); LucCount++)
        {
        }
    }
    {
        uint32 LucCount = 0;
        MCU_CLKKCPROT1 = KCPROT_ENABLE;
        MCU_CKD_SSCGC = 0x6;
        MCU_CLKKCPROT1 = KCPROT_DISABLE;
        /* check */
        for (LucCount = 0; (LucCount < 20000) && (MCU_CKD_SSCGS != 0x02UL); LucCount++)
        {
        }
    }

    {
        uint32 LucCount = 0;
        MCU_CLKKCPROT1 = KCPROT_ENABLE;
        MCU_CKS_SSCGC = 0x0;
        MCU_CLKKCPROT1 = KCPROT_DISABLE;
        /* check */
        for (LucCount = 0; LucCount < 20000; LucCount++)
        {
        }
        while((MCU_CKS_SSCGS != 0x00UL));
    }

    {
        uint32 LucCount = 0;
        MCU_CLKKCPROT1 = KCPROT_ENABLE;
        MCU_CKD_SSCGC = 0x0;
        MCU_CLKKCPROT1 = KCPROT_DISABLE;
        /* check */
        for (LucCount = 0; (LucCount < 20000) && (MCU_CKD_SSCGS != 0x02UL); LucCount++)
        {
        }
    }

    CLKKCPROT1 = KCPROT_ENABLE;
    /* Setting CAN Clock*/
    CKSC_RCANC = (0x01UL << 16) | 0x01UL;

    while (((0x01UL << 16) | 0x01UL) != CKSC_RCANS);

    /* Setting CAN Clock*/
    CKSC_MSPIC = 0x02UL;

    while ((0x02UL) != CKSC_MSPIS);
    
    CLKKCPROT1 = KCPROT_DISABLE;
}

/*****************************************************************************
--------------------- Static functions --------------------------------------
 *****************************************************************************/
