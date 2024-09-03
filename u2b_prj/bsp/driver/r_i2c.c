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

#define USE_EXT_DATATYPE
#include "r_i2c.h"
#include "r_intc.h"
#include "r_ostm.h"

/*****************************************************************************
 Macro definitions
 *****************************************************************************/
#define I2C_PORT_SDA 20
#define I2C_PORT_SCL 20
#define I2C_PIN_SDA 0U
#define I2C_PIN_SCL 1U

#define I2C_PCR_BASE 0xFFD90000
#define I2C_REG_SDA REG32(I2C_PCR_BASE+0x2000+I2C_PORT_SDA*0x40+I2C_PIN_SDA*0x4)
#define I2C_REG_SCL REG32(I2C_PCR_BASE+0x2000+I2C_PORT_SCL*0x40+I2C_PIN_SCL*0x4)


#define I2C_M_READ           0x0001 /* Read data, from slave to master */
#define I2C_M_NOSTOP         0x0040 /* Message should not end with a STOP */
#define I2C_M_NOSTART        0x0080 /* Message should not begin with a START */

/* I2c bus speed */

#define I2C_SPEED_STANDARD   100000  /* Standard speed (100Khz) */
#define I2C_SPEED_FAST       400000  /* Fast speed     (400Khz) */
#define I2C_SPEED_FAST_PLUS  1000000 /* Fast+ speed    (  1Mhz) */
#define I2C_SPEED_HIGH       3400000 /* High speed     (3.4Mhz) */

#define I2C_TRANS_START 1U
#define I2C_TRANS_ADDR  2U
#define I2C_TRANS_RW    3U
#define I2C_TRANS_R_ACK 4U
#define I2C_TRANS_T_ACK 5U
#define I2C_TRANS_STOP  6U
#define I2C_TRANS_R_DATA  7U
#define I2C_TRANS_T_DATA  8U
#define I2C_TRANS_IDLE  0U

#define I2C_MAX_MSG 2U

#define I2C_ADDR_LENGTH 7U
#define I2C_DATA_LENGTH 8U

#define I2C_ADDR_MAX_POS (I2C_ADDR_LENGTH-1)
#define I2C_DATA_MAX_POS (I2C_DATA_LENGTH-1)

#define I2C_PCR_PFC     0U
#define I2C_PCR_PFCE    1U
#define I2C_PCR_PM      4U
#define I2C_PCR_PIPC    5U
#define I2C_PCR_PMC     6U
#define I2C_PCR_PPR     8U
#define I2C_PCR_P       12U
#define I2C_PCR_PIBC    16U
#define I2C_PCR_PBDC    17U
#define I2C_PCR_PD      18U
#define I2C_PCR_PU      19U
#define I2C_PCR_PIS     20U
#define I2C_PCR_PDSC    24U
#define I2C_PCR_PUCC    25U
#define I2C_PCR_PODCE   27U
#define I2C_PCR_PODC    28U
#define I2C_PCR_PINV    30U

#define I2C_BIT_READING 0xFF
#define I2C_SET_PCR(reg,bit,mask)  reg = mask?reg|(1U<<bit):reg&(~(1U<<bit))
#define I2C_GET_PCR(reg,bit)    (reg>>bit)&0x00000001

#define I2C_ERR_NAK 1U
/*****************************************************************************
 Typedef definitions
 *****************************************************************************/
typedef struct STag_I2C_MsgSt
{
  uint8 addr;              /* Slave address (7bit) */
  uint16 flags;             /* See I2C_M_* definitions */
  uint8 *buffer;            /* Buffer to be transferred */
  uint8 length;            /* Length of the buffer in bytes */
}I2C_MsgSt;

typedef struct STag_I2C_DevSt
{
  uint8 dev_status;
  uint8 trans_status;
  uint8 err_status;
  I2C_MsgSt *msg;
  uint8 msg_length;
  uint8 msg_pos;
  uint8 byte_pos;
  uint8 bit_pos;
  uint8 operate_cnt;
}I2C_DevSt;
/*****************************************************************************
 Global variables
 *****************************************************************************/
/*****************************************************************************
 Global static variables
 *****************************************************************************/
static I2C_MsgSt GstMsg[I2C_MAX_MSG];
static I2C_DevSt GstI2CDev;
static uint32 Gu4PsrStartMask = 0x00101011;
static uint32 Gu4PsrStopMask = 0x11111000;
static uint32 Gu4PsrHighBitMask = 0x01111101;
static uint32 Gu4PsrLowBitMask = 0x00101000;
static uint32 Gu4PsrAckMask = 0x00101000;
static uint32 Gu4PsrNackMask = 0x01111101;
static uint8 Lu1RxTemp = 0;
/*****************************************************************************
 Static functions
 *****************************************************************************/
void R_I2C_PortCfg(void)
{
    I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PM,0U);
    I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PMC,0U);
    I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PU,1U);
    I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PODC,0U);
    I2C_SET_PCR(I2C_REG_SCL,I2C_PCR_PM,0U);
    I2C_SET_PCR(I2C_REG_SCL,I2C_PCR_PMC,0U);
    I2C_SET_PCR(I2C_REG_SCL,I2C_PCR_PU,1U);
    I2C_SET_PCR(I2C_REG_SCL,I2C_PCR_PODC,0U);
}
void R_I2C_PwmCfg(void)
{
    R_INTC_Init(361UL, INTC_EITB & INTC_EIMK | INTC_EIP(6UL));
    R_OSTM_InitCtl(1U, OSTM_IE);
    R_OSTM_SetCmp(1U, 79UL);
    R_OSTM_Start(1U);
}

uint16 R_I2C_Transfer(I2C_MsgSt *PstMsg,uint8 PstCnt)
{
    uint16 Lu1Ret = E_OK;
    uint8 Lu1Index = 0;
    I2C_MsgSt * LstpMsgPtr = PstMsg;
    if(GstI2CDev.dev_status == I2C_ST_IDLE)
    {
        for(Lu1Index=0;Lu1Index<PstCnt;Lu1Index++,LstpMsgPtr++)
        {
            GstMsg[Lu1Index] = *LstpMsgPtr;
        }
        GstI2CDev.msg_length = PstCnt;
        GstI2CDev.dev_status = I2C_ST_BUSY;
    }
    else
    {
        Lu1Ret = E_NOT_OK;
    }
    return Lu1Ret;
}
uint16 R_I2C_SetBit(uint32 Pu4Mask)
{
    uint16 Lu2Ret = E_OK;
    uint8 Lu1Cnt = GstI2CDev.operate_cnt;
    
    if(GstI2CDev.operate_cnt<sizeof(Pu4Mask))
    {
        I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_P,((Pu4Mask>>(Lu1Cnt*8U))&0x00000001));
        I2C_SET_PCR(I2C_REG_SCL,I2C_PCR_P,((Pu4Mask>>(Lu1Cnt*8U+4U))&0x00000001));
        GstI2CDev.operate_cnt++;
    }
    else
    {
        Lu2Ret = E_NOT_OK;
        GstI2CDev.operate_cnt = 0;
    }
    return Lu2Ret;
}
uint16 R_I2C_GetBit(uint32 *Pu4Bit)
{
    uint16 Lu2Ret = E_NOT_OK;
    *Pu4Bit = I2C_BIT_READING;
    if(GstI2CDev.operate_cnt<4U)
    {
        switch(GstI2CDev.operate_cnt)
        {
            case 0:
            I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PM,1U);
            I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PODC,1U);
            I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PIBC,1U);
            I2C_SET_PCR(I2C_REG_SCL,I2C_PCR_P,0U);
            
            break;
            case 1:
            I2C_SET_PCR(I2C_REG_SCL,I2C_PCR_P,1U);
            break;
            case 2:
            *Pu4Bit = I2C_GET_PCR(I2C_REG_SDA,I2C_PCR_PPR);
            break;
            case 3:
            I2C_SET_PCR(I2C_REG_SCL,I2C_PCR_P,0U);
            break;
            default:
            break;
        }
        GstI2CDev.operate_cnt++;
    }
    else
    {
        I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PM,0U);
        I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PODC,0U);
        I2C_SET_PCR(I2C_REG_SDA,I2C_PCR_PIBC,0U);
        Lu2Ret = E_OK;
        GstI2CDev.operate_cnt = 0;
    }
    return Lu2Ret;
}
void R_I2C_MainFunction(void)
{
    uint8 Lu1ContinueFlag = FALSE;
    if(GstI2CDev.dev_status == I2C_ST_BUSY)
    {
        do
        {
            Lu1ContinueFlag = FALSE;
            switch(GstI2CDev.trans_status)
            {
                case I2C_TRANS_IDLE:
                if(GstI2CDev.msg_length != 0)
                {
                    GstI2CDev.trans_status = I2C_TRANS_START;
                    GstI2CDev.operate_cnt = 0;
                    Lu1ContinueFlag = TRUE;
                }
                break;
                case I2C_TRANS_START:
                if(E_OK != R_I2C_SetBit(Gu4PsrStartMask))
                {
                    GstI2CDev.trans_status = I2C_TRANS_ADDR;
                    GstI2CDev.bit_pos = 0;
                    Lu1ContinueFlag = TRUE;
                }
                break;
                case I2C_TRANS_ADDR:
                if(GstI2CDev.bit_pos<I2C_ADDR_LENGTH)
                {
                    I2C_MsgSt *PstpMsg = &GstI2CDev.msg[GstI2CDev.msg_pos];
                    uint32 Pu4AddrMask;
                    Pu4AddrMask = ((PstpMsg->addr >> (I2C_ADDR_MAX_POS-GstI2CDev.bit_pos))&0x01)?Gu4PsrHighBitMask:Gu4PsrLowBitMask;
                    if(E_OK != R_I2C_SetBit(Pu4AddrMask))
                    {
                        GstI2CDev.bit_pos++;
                        Lu1ContinueFlag = TRUE;
                    }
                }
                else
                {
                    GstI2CDev.trans_status = I2C_TRANS_RW;
                    GstI2CDev.bit_pos = 0;
                    Lu1ContinueFlag = TRUE;
                }
                break;
                case I2C_TRANS_RW:
                {
                    I2C_MsgSt *PstpMsg = &GstI2CDev.msg[GstI2CDev.msg_pos];
                    uint32 Pu4RWMask;
                    Pu4RWMask = (PstpMsg->flags & I2C_M_READ)?Gu4PsrHighBitMask:Gu4PsrLowBitMask;
                    if(E_OK != R_I2C_SetBit(Pu4RWMask))
                    {
                        GstI2CDev.trans_status = I2C_TRANS_R_ACK;
                        Lu1ContinueFlag = TRUE;
                    }
                }
                break;
                case I2C_TRANS_R_ACK:
                {
                    uint32 Lu4Bit;
                    I2C_MsgSt *PstpMsg = &GstI2CDev.msg[GstI2CDev.msg_pos];
                    if(E_OK != R_I2C_GetBit(&Lu4Bit))
                    {
                        if(Lu4Bit != I2C_BIT_READING)
                        {
                            GstI2CDev.err_status |= Lu4Bit?I2C_ERR_NAK:E_OK;
                        }
                    }
                    else
                    {
                        GstI2CDev.trans_status = (PstpMsg->flags&I2C_M_READ)?I2C_TRANS_R_DATA:I2C_TRANS_T_DATA;
                    }
                }
                break;
                case I2C_TRANS_T_ACK:
                {
                    I2C_MsgSt *PstpMsg = &GstI2CDev.msg[GstI2CDev.msg_pos];
                    if(E_OK != R_I2C_SetBit(Gu4PsrAckMask))
                    {
                        GstI2CDev.trans_status = (PstpMsg->flags&I2C_M_READ)?I2C_TRANS_R_DATA:I2C_TRANS_T_DATA;
                        Lu1RxTemp = 0;
                        Lu1ContinueFlag = TRUE;
                    }
                }
                break;
                case I2C_TRANS_STOP:
                if(E_OK != R_I2C_SetBit(Gu4PsrStopMask))
                {
                    GstI2CDev.trans_status = I2C_TRANS_IDLE;
                    GstI2CDev.dev_status = I2C_ST_IDLE;
                    GstI2CDev.msg_length = 0U;
                    Lu1ContinueFlag = TRUE;
                }
                break;
                case I2C_TRANS_R_DATA:
                {
                    uint32 Lu4Bit;
                    I2C_MsgSt *PstpMsg = &GstI2CDev.msg[GstI2CDev.msg_pos];
                    if(GstI2CDev.byte_pos < PstpMsg->length)
                    {
                        if(GstI2CDev.bit_pos<I2C_DATA_LENGTH)
                        {
                            if(E_OK != R_I2C_GetBit(&Lu4Bit))
                            {
                                if(Lu4Bit != I2C_BIT_READING)
                                {
                                    uint8 Lu1Shift = I2C_DATA_MAX_POS - GstI2CDev.bit_pos;
                                    Lu1RxTemp = Lu4Bit?(Lu1RxTemp|(1U<< Lu1Shift)):(Lu1RxTemp&(~(1U<< Lu1Shift)));
                                }
                            }
                            else
                            {
                                GstI2CDev.bit_pos ++;
                            }
                        }
                        else
                        {
                            PstpMsg->buffer[GstI2CDev.byte_pos] = Lu1RxTemp;
                            GstI2CDev.bit_pos = 0;
                            GstI2CDev.byte_pos ++;
                            Lu1ContinueFlag = TRUE;
                            GstI2CDev.trans_status = I2C_TRANS_T_ACK;
                        }
                    }
                    else
                    {
                        GstI2CDev.trans_status = (PstpMsg->flags & I2C_M_NOSTOP)?((GstI2CDev.msg_pos<(GstI2CDev.msg_length-1))? \
                                                GstI2CDev.msg_pos++,I2C_TRANS_START:I2C_TRANS_STOP):I2C_TRANS_STOP;
                        GstI2CDev.byte_pos = 0;
                        Lu1ContinueFlag = TRUE;
                    }
                }
                break;
                case I2C_TRANS_T_DATA:
                {
                    I2C_MsgSt *PstpMsg = &GstI2CDev.msg[GstI2CDev.msg_pos];
                    if(GstI2CDev.byte_pos < PstpMsg->length)
                    {
                        if(GstI2CDev.bit_pos<I2C_DATA_LENGTH)
                        {
                            uint32 Pu4TDataMask;
                            Pu4TDataMask = ((PstpMsg->buffer[GstI2CDev.byte_pos] >> (I2C_DATA_MAX_POS-GstI2CDev.bit_pos))&0x01)?\
                                            Gu4PsrHighBitMask:Gu4PsrLowBitMask;
                            if(E_OK != R_I2C_SetBit(Pu4TDataMask))
                            {
                                GstI2CDev.bit_pos++;
                                Lu1ContinueFlag = TRUE;
                            }
                        }
                        else
                        {
                            GstI2CDev.bit_pos = 0;
                            GstI2CDev.byte_pos ++;
                            Lu1ContinueFlag = TRUE;
                            GstI2CDev.trans_status = I2C_TRANS_R_ACK;
                        }
                    }
                    else
                    {   
                        GstI2CDev.trans_status = (PstpMsg->flags & I2C_M_NOSTOP)?((GstI2CDev.msg_pos<(GstI2CDev.msg_length-1))? \
                                                GstI2CDev.msg_pos++,I2C_TRANS_START:I2C_TRANS_STOP):I2C_TRANS_STOP;
                        GstI2CDev.byte_pos = 0;
                        Lu1ContinueFlag = TRUE;
                    }
                }
                break;
                default:
                break;
            }
        }while(Lu1ContinueFlag == TRUE);
    }

}
/*****************************************************************************
--------------------- Global functions --------------------------------------
 *****************************************************************************/
uint16 R_I2C_Init(void)
{
    R_I2C_PortCfg();
    GstI2CDev.dev_status = I2C_ST_UNINIT;
    GstI2CDev.trans_status = I2C_TRANS_IDLE;
    GstI2CDev.err_status = 0U;
    GstI2CDev.msg = &GstMsg[0];
    GstI2CDev.msg_length = 0U;
    GstI2CDev.msg_pos = 0U;
    GstI2CDev.byte_pos = 0U;
    GstI2CDev.bit_pos = 0U;
    GstI2CDev.dev_status = I2C_ST_IDLE;
    R_I2C_PwmCfg();
    return E_OK;
}
uint16 R_I2C_Write(uint8 Pu1Addr,
                   uint8* Pu1pBuffer,uint8 Pu1BufLen)
{
    uint16 Lu2Ret;
    I2C_MsgSt LstMsg;
    LstMsg.addr = Pu1Addr;
    LstMsg.flags = 0U;
    LstMsg.buffer = Pu1pBuffer;
    LstMsg.length = Pu1BufLen;
    Lu2Ret = R_I2C_Transfer(&LstMsg,1U);
    return Lu2Ret;
}
uint16 R_I2C_Read(uint8 Pu1Addr,
                  uint8* Pu1pBuffer,uint8 Pu1BufLen)
{
    uint16 Lu2Ret;
    I2C_MsgSt LstMsg;
    LstMsg.addr = Pu1Addr;
    LstMsg.flags = I2C_M_READ;
    LstMsg.buffer = Pu1pBuffer;
    LstMsg.length = Pu1BufLen;
    Lu2Ret = R_I2C_Transfer(&LstMsg,1U);
    return Lu2Ret;
}
uint16 R_I2C_WriteRead(uint8 Pu1Addr,
                       uint8* Pu1pWBuffer,uint8 Pu1WBufLen,
                       uint8* Pu1pRBuffer,uint8 Pu1RBufLen)
{
    uint16 Lu2Ret;
    I2C_MsgSt LstMsg[2];
    LstMsg[0].addr = Pu1Addr;
    LstMsg[0].flags = I2C_M_NOSTOP;
    LstMsg[0].buffer = Pu1pWBuffer;
    LstMsg[0].length = Pu1WBufLen;

    LstMsg[1].addr = Pu1Addr;
    LstMsg[1].flags = I2C_M_READ;
    LstMsg[1].buffer = Pu1pRBuffer;
    LstMsg[1].length = Pu1RBufLen;

    Lu2Ret = R_I2C_Transfer(&LstMsg[0],2U);
    return Lu2Ret;
}

uint8 R_I2C_GetDevStatus(void)
{
    return GstI2CDev.dev_status;
}
