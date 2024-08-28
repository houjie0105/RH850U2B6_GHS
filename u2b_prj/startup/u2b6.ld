/*===========================================================================*/
/* Module       = u2b6.ld                                                    */
/* SW-VERSION   = CN1.0                                                      */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* (c) 2018-2020 Renesas Electronics Corporation. All rights reserved.       */
/*===========================================================================*/
/* Purpose:     Template of common linker directive file                     */
/*                                                                           */
/*===========================================================================*/
/*                                                                           */
/* Warranty Disclaimer                                                       */
/*                                                                           */
/* Because the Product(s) is licensed free of charge, there is no warranty   */
/* of any kind whatsoever and expressly disclaimed and excluded by Renesas,  */
/* either expressed or implied, including but not limited to those for       */
/* non-infringement of intellectual property, merchantability and/or         */
/* fitness for the particular purpose.                                       */
/* Renesas shall not have any obligation to maintain, service or provide bug */
/* fixes for the supplied Product(s) and/or the Application.                 */
/*                                                                           */
/* Each User is solely responsible for determining the appropriateness of    */
/* using the Product(s) and assumes all risks associated with its exercise   */
/* of rights under this Agreement, including, but not limited to the risks   */
/* and costs of program errors, compliance with applicable laws, damage to   */
/* or loss of data, programs or equipment, and unavailability or             */
/* interruption of operations.                                               */
/*                                                                           */
/* Limitation of Liability                                                   */
/*                                                                           */
/* In no event shall Renesas be liable to the User for any incidental,       */
/* consequential, indirect, or punitive damage (including but not limited    */
/* to lost profits) regardless of whether such liability is based on breach  */
/* of contract, tort, strict liability, breach of warranties, failure of     */
/* essential purpose or otherwise and even if advised of the possibility of  */
/* such damages. Renesas shall not be liable for any services or products    */
/* provided by third party vendors, developers or consultants identified or  */
/* referred to the User by Renesas in connection with the Product(s) and/or  */
/* the Application.                                                          */
/*                                                                           */
/*===========================================================================*/
/* Environment:                                                              */
/*              Device:         U2B6                                         */
/*              IDE:            GHS Multi for V800  V7.1.6/2018.1.5          */
/*===========================================================================*/

MEMORY
{
  /* ------------------------------------------------------------------------ */
  /*                            Code flash                                    */
  /* ------------------------------------------------------------------------ */

  /* User Area */
  CFUAB      : ORIGIN = 0x00000000,  LENGTH = 6M
  CFUAB_TOP  : ORIGIN = .         ,  LENGTH = 0

  /* User Boot Area */
  CFUBAB     : ORIGIN = 0x08000000,  LENGTH = 64K
  CFUBAB_TOP : ORIGIN = .         ,  LENGTH = 0

  /* ------------------------------------------------------------------------ */
  /*                            Data flash                                    */
  /* ------------------------------------------------------------------------ */

  /* Data Area */
  DFDA0      : ORIGIN = 0xFF200000,  LENGTH = 64K
  DFDA1      : ORIGIN = 0xFF210000,  LENGTH = 64K
  DFDA2      : ORIGIN = 0xFF220000,  LENGTH = 64K

  /* Hardware Property Area */
  DFHPA0EDA  : ORIGIN = 0xFF320000,  LENGTH = 2K
  DFHPA0CSA  : ORIGIN = 0xFF320800,  LENGTH = 4K
  DFHPA0SSA  : ORIGIN = 0xFF321800,  LENGTH = 4K
  DFHPA0BPA0 : ORIGIN = 0xFF322800,  LENGTH = 4K
  DFHPA0ECA0 : ORIGIN = 0xFF325000,  LENGTH = 6K
  DFHPA0ECA1 : ORIGIN = 0xFF326800,  LENGTH = 6K

  /* ------------------------------------------------------------------------ */
  /*                            Local RAM                                     */
  /* ------------------------------------------------------------------------ */

  LR_SELF    : ORIGIN = 0xFDE00000,  LENGTH = 62K
  LR_SELF_TOP: ORIGIN = .         ,  LENGTH = 0
  LR_CPU0    : ORIGIN = 0xFDC00000,  LENGTH = 64K
  LR_CPU0_TOP: ORIGIN = .         ,  LENGTH = 0
  LR_CPU1    : ORIGIN = 0xFDA00000,  LENGTH = 64K
  LR_CPU1_TOP: ORIGIN = .         ,  LENGTH = 0
  LR_CPU2    : ORIGIN = 0xFD800000,  LENGTH = 64K
  LR_CPU2_TOP: ORIGIN = .         ,  LENGTH = 0

  /* ------------------------------------------------------------------------ */
  /*                           Cluster RAM                                    */
  /* ------------------------------------------------------------------------ */

  CRAM0      : ORIGIN = 0xFE000000,  LENGTH = 384K
  CRAM0_TOP  : ORIGIN = .         ,  LENGTH = 0

  /* ------------------------------------------------------------------------ */
  /*                                Stack                                     */
  /* ------------------------------------------------------------------------ */

  STACK      : ORIGIN = 0xFDE0F800,  LENGTH = 2K
}

SECTIONS
{
  /* Start of internal ROM area (CFUAB) */

  .intvect 0x0000          align(16)            :> CFUAB   /* start of interrupt vector. This section contains RESET and exception table for PE0 */
  .inttable                align(0x200)         :>.
  .inttable_end                                 :>.

  .rozdata                                      :>.         /* constant datas in ZDA area */
  .robase                      align(4)         :>.         /* initialize textpointer TP for SDA addressing */
  .rosdata                     align(4)         :>.         /* constant datas in SDA area */
  .rodata                      align(4)         :>.         /* constant datas in normal area */

  /* program code area */
  .text     align(4)                            :>.

  .fixaddr              align(4)                :>.     /* ghs internal (compiler) */
  .fixtype              align(4)                :>.     /* ghs internal (compiler) */
  .secinfo              align(4)                :>.     /* ghs internal (runtime library) */
  .syscall              align(4)                :>.     /* ghs internal (linker) */

  .romdata              ROM(.data)              :>.     /* constant data to initialize variables (copied to RAM at startup)*/
  .romzdata             ROM(.zdata)             :>.     /* constant data to initialize variables in ZDA area (copied to RAM at startup)*/
  .romsdata             ROM(.sdata)             :>.     /* constant data to initialize variables in SDA area (copied to RAM at startup)*/
  .romtdata             ROM(.tdata)             :>.     /* constant data to initialize variables in TDA area (copied to RAM at startup)*/
  .romsldata            ROM(.sldata)            :>.     /* constant data to initialize variables in LSDA area (copied to GRAM at startup)*/

  /* Start of internal RAM area (iRAM) */
  .data                        align(4)         :> CRAM0      /* initialized data */
  .sdabase                     align(4)         :>.      /* initialize global pointer GP for SDA addressing */
  .zdata                       align(4)         :>.      /* initialized data in ZDA area */
  .sldata                      align(4)         :>.      /* initialized data in LSDA area */
  .sdata                       align(4)         :>.      /* initialized data in SDA area */

  .bss                         align(4)         :>.      /* zero initialized data */
  .zbss                        align(4)         :>.      /* zero initialized data in ZDA area */
  .slbss                       align(4)         :>.      /* zero initialized data in LSDA area */
  .sbss                        align(4)         :>.      /* zero initialized data in SDA area */

  .ramtext  align(128)  MAX_SIZE(0x2000)        :>.      /* initialized and zero-initialized data in TDA area */
  .tdata    align(4)    MAX_SIZE(0x0100)        :>.      /* initialized and zero-initialized data in TDA area */
  .exec     align(128)                          :>.
  .mev_address                 align(32)        :>.      /* Mutual Exclusive Variable is used for Sync flags between multi core */
  .FLASH_OPS_CODE            0xFE010000         :>.

  .heapbase align(4)                            :>.
  .heap     align(4)
            pad(addr(CRAM0_TOP)-addr(.heapbase))
            NOCLEAR                             :>.      /* definition of heap size */

  .top_of_RAM           align(4)                :> CRAM0_TOP

  .stack    align(4)    pad(0x0800)             :> STACK      /* definition of stack size */

  OCDID_SECTION    0xFF322320  align(4)           :> DFHPA0SSA
  SPID_SECTION     0xFF322340  align(4)           :> .
  SACON_SECTION      0xFF322700  align(4)         :> .

  /*  Symbols for compiler references */
  ___ghs_romstart       = MEMADDR(CFUAB);
  ___ghs_romend         = MEMENDADDR(CFUAB);

  ___ghs_ramstart       = MEMADDR(LR_CPU0);
  ___ghs_ramend         = MEMENDADDR(LR_CPU0);

  ___ghs_ram1start      = MEMADDR(LR_CPU1);
  ___ghs_ram1end        = MEMENDADDR(LR_CPU1);

  ___ghs_ram1start      = MEMADDR(LR_CPU2);
  ___ghs_ram1end        = MEMENDADDR(LR_CPU2);

  ___ghs_cram0start     = MEMADDR(CRAM0);
  ___ghs_cram0end       = MEMENDADDR(CRAM0);
}

/*===========================================================================*/
/*      End of File                                                          */
/*===========================================================================*/
