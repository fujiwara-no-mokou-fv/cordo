#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include"../inc/cpu.h"
#include"../inc/rv_cpu.h"
#include"../inc/rv_disp.h"



//                      [   f7  | rs2 | rs1 |f3 | rd  |opcode ]
const uint32_t instr = 0b0000000'00101'00110'000'01010'0110011;

/* 
 * this is instruction add a0, t0, t1
 *
 * f7 aka func7 = 0b0000000 (for add)
 * rs2 aka register source 2 = 0b00101 (5 in decimal, index x5 and name t0)
 *   prefix 't' in name mean that register is temporary  - function call can overwrites it
 *   but this is a convension
 * rs1 obviusly = 0b00110 (6 in decimal, index x6 and name t1)
 * f3 aka func3 = 0b000 (for add)
 * rd aka register destination = 0b01010 (10 in decimal, index x10 and name a0)
 *   prefix 'a' in name mean that register is argument funcition 
 *   (or return value if index x10/x11)
 *
 * opcode = 0b0110011 (0x33 in nexadecimal)
 * opcode 0x33 for register-to-register instructions (add, sub, xor ...)
 * opcode 0x13 for  registre-to-immediate instructions (addi, xori, ori ...)
 *
 * descriptin instruction (C)
 * add: rd = rs1 + rs2
 * addi: rd = rs1 + imm
 *
 * sescription instrucion (asm)
 * add rd, rs1, rs2
 * addi rd, rs1, imm 
 *
 * exampels (asm):
 * index can be used insted of name
 * add x1, x2, x3 mean x1 = x2 + x3
 * this is the same as:
 * add ra, sp, gp
 *  some registers have non-obviusly names such as:
 *  zero - (always equal to 0, index x0 the only special register 0 out of 32 [x0 .. x31])
 *  ra - return addres
 *  sp - stack pointer
 *  gp - global pointer
 *
 *  but with the exception of the zero registre, all are general-porpouse registers
 *  general-porpouse register - they are no different from each other
 *
 *  naming is determining by convention
 *
 *  addi t3, t2, 10 mean t3 = t2 + 10
 *   subi dose not exist
 *  addi t3, t2, -10 mean t3 = t2 + (-10)
 *   move also dose not exist
 *  add t0, t1, zero mean t0 = t1 + zero or t0 = t1
 *   there is an mv pseudo instruction it is completely equivaent add rd, rs1, zero
 *   identical machine code
 *
 * more in RISC-V_CARD and riscv-unprivileged
*/

int main () {

  cpu_t vm;
  
  rv_cpu_arch_desc_t rv_desc_32i = {
    .dispatcher = rv_32i_dispatcher,
    .mem_sz = 4096U,
    .pc = 32U
  };

  cpu_arch_t rv32i = {
    .desc = (cpu_arch_desc_t)&rv_desc_32i,
    .init = init_rv_cpu,
    .exec = exec_rv_cpu,
    .free = free_rv_cpu
  };

  init_cpu(&vm, &rv32i);

  ((rv_cpu_state_t *)vm.state)->x_reg[3] = 256;
  ((rv_cpu_state_t *)vm.state)->x_reg[4] = 768;
  ((rv_cpu_state_t *)vm.state)->x_reg[5] = 0xDEADBEAF;
  memcpy(&((rv_cpu_state_t *)vm.state)->mem[32], &instr, 4);

  exec_cpu(&vm);

  if(((rv_cpu_state_t *)vm.state)->x_reg[5] != 0xDEADBEAF
  && ((rv_cpu_state_t *)vm.state)->x_reg[5] == 1024){
    puts("success");
  }

  free_cpu(&vm);

  return 0;
}
