#include "freertos/xtensa_rtos.h"
#include "esp_private/panic_reason.h"
#include "soc/soc.h"

#include "sdkconfig.h"



/*
--------------------------------------------------------------------------------
  Panic handler.
  Should be reached by call0 (preferable) or jump only. If call0, a0 says where
  from. If on simulator, display panic message and abort, else loop indefinitely.
--------------------------------------------------------------------------------
*/

    .section .iram1,"ax"
    .global panicHandler

    .global     _xt_panic
    .type       _xt_panic,@function
    .align      4
    .literal_position
    .align      4

_xt_panic:
    /* Allocate exception frame and save minimal context. */
    addi    sp, sp, -XT_STK_FRMSZ
  
    s32i    a0, sp, XT_STK_A0
    s32i    a1, sp, XT_STK_A1
    rsr     a0, PS                          /* save interruptee's PS */
    s32i    a0, sp, XT_STK_PS
    rsr     a0,(EPC + XCHAL_DEBUGLEVEL)    /* save interruptee's PC */
    s32i    a0, sp, XT_STK_PC
    s32i    a2,  sp, XT_STK_A2
    s32i    a3,  sp, XT_STK_A3
    s32i    a4,  sp, XT_STK_A4
    s32i    a5,  sp, XT_STK_A5
    s32i    a6,  sp, XT_STK_A6
    s32i    a7,  sp, XT_STK_A7
    s32i    a8,  sp, XT_STK_A8
    s32i    a9,  sp, XT_STK_A9
    s32i    a10, sp, XT_STK_A10
    s32i    a11, sp, XT_STK_A11
    s32i    a12, sp, XT_STK_A12
    s32i    a13, sp, XT_STK_A13
    /*
    Call0 ABI callee-saved regs a12-15 do not need to be saved here.
    a12-13 are the caller's responsibility so it can use them as scratch.
    So only need to save a14-a15 here for Windowed ABI (not Call0).
    */
  //  #ifndef __XTENSA_CALL0_ABI__
    s32i    a14, sp, XT_STK_A14
    s32i    a15, sp, XT_STK_A15
   // #endif


    /* Set up PS for C, disable all interrupts except NMI and debug, and clear EXCM. */
    movi    a0, PS_INTLEVEL(5) | PS_UM | PS_WOE
    wsr     a0, PS


    mov     a6, sp
   // l32r	  a7, 42000928
   // li a0, 0x42000928
    //movi    a11, panicHandler
    //callx4  a11
   // l32i.n	a8, sp, 4
    //addi.n	a8, a8, 3
    //s32i.n	a8, sp, 4
    call4   panicHandler                 /* Call interrupt handler */
  
    l32i    a0, sp, XT_STK_PC
    wsr     a0,(EPC + XCHAL_DEBUGLEVEL)
    l32i    a0, sp, XT_STK_PS
    wsr     a0,PS


    l32i    a0, sp, XT_STK_A0
    l32i    a1, sp, XT_STK_A1
    l32i    a2,  sp, XT_STK_A2
    l32i    a3,  sp, XT_STK_A3
    l32i    a4,  sp, XT_STK_A4
    l32i    a5,  sp, XT_STK_A5
    l32i    a6,  sp, XT_STK_A6
    l32i    a7,  sp, XT_STK_A7
    l32i    a8,  sp, XT_STK_A8
    l32i    a9,  sp, XT_STK_A9
    l32i    a10, sp, XT_STK_A10
    l32i    a11, sp, XT_STK_A11
 
    l32i    a12, sp, XT_STK_A12
    l32i    a13, sp, XT_STK_A13
   // #ifndef __XTENSA_CALL0_ABI__
    l32i    a14, sp, XT_STK_A14
    l32i    a15, sp, XT_STK_A15
  //  #endif

    addi    sp, sp, XT_STK_FRMSZ        /* Restore SP*/
    rfi     XCHAL_DEBUGLEVEL       // Return from high-level interrupt








