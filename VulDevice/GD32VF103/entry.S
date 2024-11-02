// See LICENSE for license details

#ifndef ENTRY_S
#define ENTRY_S

#include "riscv_encoding.h"
#include "riscv_bits.h"
#include "n200_eclic.h"
#include "n200_timer.h"

###############################################
###############################################
# Disable Interrupt
#
.macro DISABLE_MIE
  csrc CSR_MSTATUS, MSTATUS_MIE  
.endm


###############################################
###############################################
#Save caller registers
.macro SAVE_CONTEXT

#ifdef __riscv_flen
  #if (__riscv_flen==64 )
    addi sp, sp, -20*REGBYTES - 20*FPREGBYTES
  #else
    addi sp, sp, -20*REGBYTES
  #endif
#else
  addi sp, sp, -20*REGBYTES
#endif
  STORE x1, 0*REGBYTES(sp)
	STORE x4, 1*REGBYTES(sp)
	STORE x5, 2*REGBYTES(sp)
	STORE x6, 3*REGBYTES(sp)
	STORE x7, 4*REGBYTES(sp)
	STORE x10, 5*REGBYTES(sp)
	STORE x11, 6*REGBYTES(sp)
	STORE x12, 7*REGBYTES(sp)
	STORE x13, 8*REGBYTES(sp)
	STORE x14, 9*REGBYTES(sp)
	STORE x15, 10*REGBYTES(sp)
#ifndef __riscv_32e
  STORE x16, 11*REGBYTES(sp)
	STORE x17, 12*REGBYTES(sp)
	STORE x28, 13*REGBYTES(sp)
	STORE x29, 14*REGBYTES(sp)
	STORE x30, 15*REGBYTES(sp)
	STORE x31, 16*REGBYTES(sp)
#endif

#ifdef __riscv_flen
  #if (__riscv_flen == 64)
  FPSTORE f0, (20*REGBYTES + 0*FPREGBYTES)(sp)
	FPSTORE f1, (20*REGBYTES + 1*FPREGBYTES)(sp)
	FPSTORE f2, (20*REGBYTES + 2*FPREGBYTES)(sp)
	FPSTORE f3, (20*REGBYTES + 3*FPREGBYTES)(sp)
	FPSTORE f4, (20*REGBYTES + 4*FPREGBYTES)(sp)
	FPSTORE f5, (20*REGBYTES + 5*FPREGBYTES)(sp)
	FPSTORE f6, (20*REGBYTES + 6*FPREGBYTES)(sp)
	FPSTORE f7, (20*REGBYTES + 7*FPREGBYTES)(sp)
	FPSTORE f10, (20*REGBYTES + 8*FPREGBYTES)(sp)
	FPSTORE f11, (20*REGBYTES + 9*FPREGBYTES)(sp)
	FPSTORE f12, (20*REGBYTES + 10*FPREGBYTES)(sp)
	FPSTORE f13, (20*REGBYTES + 11*FPREGBYTES)(sp)
	FPSTORE f14, (20*REGBYTES + 12*FPREGBYTES)(sp)
	FPSTORE f15, (20*REGBYTES + 13*FPREGBYTES)(sp)
	FPSTORE f16, (20*REGBYTES + 14*FPREGBYTES)(sp)
	FPSTORE f17, (20*REGBYTES + 15*FPREGBYTES)(sp)
	FPSTORE f28, (20*REGBYTES + 16*FPREGBYTES)(sp)
	FPSTORE f29, (20*REGBYTES + 17*FPREGBYTES)(sp)
	FPSTORE f30, (20*REGBYTES + 18*FPREGBYTES)(sp)
	FPSTORE f31, (20*REGBYTES + 19*FPREGBYTES)(sp)   
  #endif    
#endif


.endm


###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT
    LOAD x1, 0*REGBYTES(sp)
	LOAD x4, 1*REGBYTES(sp)
	LOAD x5, 2*REGBYTES(sp)
	LOAD x6, 3*REGBYTES(sp)
	LOAD x7, 4*REGBYTES(sp)
	LOAD x10, 5*REGBYTES(sp)
	LOAD x11, 6*REGBYTES(sp)
	LOAD x12, 7*REGBYTES(sp)
	LOAD x13, 8*REGBYTES(sp)
	LOAD x14, 9*REGBYTES(sp)
	LOAD x15, 10*REGBYTES(sp)
#ifndef __riscv_32e
    LOAD x16, 11*REGBYTES(sp)
	LOAD x17, 12*REGBYTES(sp)
	LOAD x28, 13*REGBYTES(sp)
	LOAD x29, 14*REGBYTES(sp)
	LOAD x30, 15*REGBYTES(sp)
	LOAD x31, 16*REGBYTES(sp)
#endif


#ifdef __riscv_flen
  #if (__riscv_flen==64)
/* Restore fp caller registers */
	FPLOAD f0, (20*REGBYTES + 0*FPREGBYTES)(sp)
	FPLOAD f1, (20*REGBYTES + 1*FPREGBYTES)(sp)
	FPLOAD f2, (20*REGBYTES + 2*FPREGBYTES)(sp)
	FPLOAD f3, (20*REGBYTES + 3*FPREGBYTES)(sp)
	FPLOAD f4, (20*REGBYTES + 4*FPREGBYTES)(sp)
	FPLOAD f5, (20*REGBYTES + 5*FPREGBYTES)(sp)
	FPLOAD f6, (20*REGBYTES + 6*FPREGBYTES)(sp)
	FPLOAD f7, (20*REGBYTES + 7*FPREGBYTES)(sp)
	FPLOAD f10, (20*REGBYTES + 8*FPREGBYTES)(sp)
	FPLOAD f11, (20*REGBYTES + 9*FPREGBYTES)(sp)
	FPLOAD f12, (20*REGBYTES + 10*FPREGBYTES)(sp)
	FPLOAD f13, (20*REGBYTES + 11*FPREGBYTES)(sp)
	FPLOAD f14, (20*REGBYTES + 12*FPREGBYTES)(sp)
	FPLOAD f15, (20*REGBYTES + 13*FPREGBYTES)(sp)
	FPLOAD f16, (20*REGBYTES + 14*FPREGBYTES)(sp)
	FPLOAD f17, (20*REGBYTES + 15*FPREGBYTES)(sp)
	FPLOAD f28, (20*REGBYTES + 16*FPREGBYTES)(sp)
	FPLOAD f29, (20*REGBYTES + 17*FPREGBYTES)(sp)
	FPLOAD f30, (20*REGBYTES + 18*FPREGBYTES)(sp)
	FPLOAD f31, (20*REGBYTES + 19*FPREGBYTES)(sp)
  #endif
#endif


#ifdef __riscv_flen
  #if(__riscv_flen == 64 )
    addi sp, sp, 20*REGBYTES  + 20*FPREGBYTES
  #else
    addi sp, sp, 20*REGBYTES
  #endif
#else
// De-allocate the stack space
 addi sp, sp, 20*REGBYTES
#endif
.endm

###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT_EXCPT_X5
  LOAD x1,  0*REGBYTES(sp)
  LOAD x6,  2*REGBYTES(sp)
  LOAD x7,  3*REGBYTES(sp)
  LOAD x10, 4*REGBYTES(sp)
  LOAD x11, 5*REGBYTES(sp)
  LOAD x12, 6*REGBYTES(sp)
  LOAD x13, 7*REGBYTES(sp)
  LOAD x14, 8*REGBYTES(sp)
  LOAD x15, 9*REGBYTES(sp)
#ifndef __riscv_32e
  LOAD x16, 10*REGBYTES(sp)
  LOAD x17, 11*REGBYTES(sp)
  LOAD x28, 12*REGBYTES(sp)
  LOAD x29, 13*REGBYTES(sp)
  LOAD x30, 14*REGBYTES(sp)
  LOAD x31, 15*REGBYTES(sp)
#endif
.endm

###############################################
###############################################
#restore caller registers
.macro RESTORE_CONTEXT_ONLY_X5
  LOAD x5,  1*REGBYTES(sp)
.endm

###############################################
###############################################
# Save the mepc and mstatus
#
.macro SAVE_EPC_STATUS
  csrr x5, CSR_MEPC  
  STORE x5,  16*REGBYTES(sp)
  csrr x5, CSR_MSTATUS  
  STORE x5,  17*REGBYTES(sp)
  csrr x5, CSR_MSUBM  
  STORE x5,  18*REGBYTES(sp)
.endm

###############################################
###############################################
# Restore the mepc and mstatus
#
.macro RESTORE_EPC_STATUS
  LOAD x5,  16*REGBYTES(sp)
  csrw CSR_MEPC, x5  
  LOAD x5,  17*REGBYTES(sp)
  csrw CSR_MSTATUS, x5  
  LOAD x5,  18*REGBYTES(sp)
  csrw CSR_MSUBM, x5  
.endm



###############################################
###############################################
// Trap entry point
//
  .section      .text.trap	
  .align 6// In CLIC mode, the trap entry must be 64bytes aligned
  .global trap_entry
.weak trap_entry
trap_entry:
    /* save thread context to thread stack */

    addi sp, sp, -16 * REGBYTES

    STORE x1,   1 * REGBYTES(sp)

    csrr  x1, mstatus
    STORE x1,   2 * REGBYTES(sp)

    csrr  x1, mepc
    STORE x1,   0 * REGBYTES(sp)//sw	ra,0(sp)

    STORE x4,   4 * REGBYTES(sp)
    STORE x5,   5 * REGBYTES(sp)
    STORE x6,   6 * REGBYTES(sp)
    STORE x7,   7 * REGBYTES(sp)
    STORE x8,   8 * REGBYTES(sp)
    STORE x9,   9 * REGBYTES(sp)
    STORE x10, 10 * REGBYTES(sp)
    STORE x11, 11 * REGBYTES(sp)
    STORE x12, 12 * REGBYTES(sp)
    STORE x13, 13 * REGBYTES(sp)
    STORE x14, 14 * REGBYTES(sp)
    STORE x15, 15 * REGBYTES(sp)
    /* handle interrupt */
    move  s0,sp
    csrr  a0, mepc
    mv    a1, s0
    call  handle_trap // 0x08010000
   // li a3,0x20005000
   // jalr  ra, a3, 0

  //  lw	a5,0(a1)
   // addi	a5,a5,2
  //  sw	a5,0(a1)
    LOAD  a0,   0 * REGBYTES(sp)
    csrw  mepc, a0
    LOAD  x1,   1 * REGBYTES(sp)
    LOAD  t0,   2 * REGBYTES(sp)
    csrs  mstatus, t0
    LOAD  x4,   4 * REGBYTES(sp)
    LOAD  x5,   5 * REGBYTES(sp)
    LOAD  x6,   6 * REGBYTES(sp)
    LOAD  x7,   7 * REGBYTES(sp)
    LOAD  x8,   8 * REGBYTES(sp)
    LOAD  x9,   9 * REGBYTES(sp)
    LOAD  x10, 10 * REGBYTES(sp)
    LOAD  x11, 11 * REGBYTES(sp)
    LOAD  x12, 12 * REGBYTES(sp)
    LOAD  x13, 13 * REGBYTES(sp)
    LOAD  x14, 14 * REGBYTES(sp)
    LOAD  x15, 15 * REGBYTES(sp)

    addi  sp, sp, 16 * REGBYTES

    mret


###############################################
###############################################
// IRQ entry point
//
  .section      .text.irq	
  .align 2
  .global irq_entry
.weak irq_entry
irq_entry: // -------------> This label will be set to MTVT2 register
  // Allocate the stack space
  

  SAVE_CONTEXT// Save 16 regs

  //------This special CSR read operation, which is actually use mcause as operand to directly store it to memory
  csrrwi  x0, CSR_PUSHMCAUSE, 17
  //------This special CSR read operation, which is actually use mepc as operand to directly store it to memory
  csrrwi  x0, CSR_PUSHMEPC, 18
  //------This special CSR read operation, which is actually use Msubm as operand to directly store it to memory
  csrrwi  x0, CSR_PUSHMSUBM, 19
 
service_loop:
  //------This special CSR read/write operation, which is actually Claim the CLIC to find its pending highest
  // ID, if the ID is not 0, then automatically enable the mstatus.MIE, and jump to its vector-entry-label, and
  // update the link register 
  csrrw ra, CSR_JALMNXTI, ra 
  
  //RESTORE_CONTEXT_EXCPT_X5

  #---- Critical section with interrupts disabled -----------------------
  DISABLE_MIE # Disable interrupts 

  LOAD x5,  19*REGBYTES(sp)
  csrw CSR_MSUBM, x5  
  LOAD x5,  18*REGBYTES(sp)
  csrw CSR_MEPC, x5  
  LOAD x5,  17*REGBYTES(sp)
  csrw CSR_MCAUSE, x5  


  RESTORE_CONTEXT

  
  // Return to regular code
  mret


#endif
