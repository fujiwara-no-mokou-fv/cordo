#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include"../inc/cpu.h"
#include"../inc/rv_cpu.h"
#include"../inc/rv_disp.h"



const uint32_t instr = 0b0000000'00101'00110'000'01010'0110011;

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
