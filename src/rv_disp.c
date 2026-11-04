#include<stdint.h>
#include<string.h>
#include"../inc/rv_cpu.h"
#include"../inc/rv_disp.h"
#include"../inc/rv_types.h"



void halt(cpu_state_t state, instr_t instr) {
  return;
}

void add(cpu_state_t state, instr_t instr) { 
  auto rv_state = (rv_cpu_state_t *)state;
  field_t rd = get_field(instr, RD);
  field_t rs1 = get_field(instr, RS1);
  field_t rs2 = get_field(instr, RS2);
  
  if(rd != 0) {
  rv_state->x_reg[rd] = rv_state->x_reg[rs1] + rv_state->x_reg[rs2];
  }

  rv_state->pc += sizeof(instr_t);
  instr_t next_instr = fetch_rv_instr(rv_state);
  
  [[gnu::musttail]] return rv_state->dispatcher(
      rv_state,
      next_instr);
}

void addi(cpu_state_t state, instr_t instr) {
  auto rv_state = (rv_cpu_state_t *)state;
  field_t rd = get_field(instr, RD);
  field_t rs1 = get_field(instr, RS1);
  rv_imm_t imm;
  imm.raw = get_field(instr, I_IMM);
  rv_i_imm_t i_imm = extr_i_imm(imm);

  if(rd != 0) {
  rv_state->x_reg[rd] = rv_state->x_reg[rs1] + i_imm.val;
  }

  rv_state->pc += sizeof(instr_t);

  instr_t next_instr = fetch_rv_instr(rv_state);
  
  [[gnu::musttail]] return rv_state->dispatcher(
    rv_state,
    next_instr);
}

NODE(rv_dispatch_0x33, F3F7B5,
    [0 ... 15] = halt,
    [0x0] = add
    )

NODE(rv_dispatch_0x13, F3,
    [0 ... 7] = halt,
    [0x0] = addi
    )

NODE(rv_32i_dispatcher, OP,
    [0 ... 127] = halt,
    [0x33] = rv_dispatch_0x33,
    [0x13] = rv_dispatch_0x13
    )

