#pragma once
#include<stdint.h>
#include<string.h>
#include"rv_cpu.h"
#include"rv_types.h"



typedef struct {
  size_t lsb;
  size_t msb;
} field_desc_t;

static constexpr field_desc_t OP = {
  .lsb = 0,
  .msb = 6
};

static constexpr field_desc_t RD = {
  .lsb = 7,
  .msb = 11
};

static constexpr field_desc_t F3 = {
  .lsb = 12,
  .msb = 14
};

static constexpr field_desc_t RS1 = {
  .lsb = 15,
  .msb = 19
};

static constexpr field_desc_t RS2 = {
  .lsb = 20,
  .msb = 24
};

static constexpr field_desc_t F7 = {
  .lsb = 25,
  .msb = 31
};

static constexpr field_desc_t I_IMM = {
  .lsb = 20,
  .msb = 31
};

[[gnu::always_inline]] static inline field_t get_field(
    instr_t instr,
    field_desc_t desc){
  field_t field;
  field = (instr.raw >> desc.lsb) & ((1U << ((desc.msb - desc.lsb) + 1)) - 1U);
  
  return field;
}

#define GET_OP(instr)\
  get_field(instr, OP)

#define GET_F3(instr)\
  get_field(instr, F3)

#define GET_F3F7B5(instr)\
  get_field(instr, F3) |\
  get_field(instr, (field_desc_t){30, 30}) << 3U

#define NODE(name, field, ...)\
  void name(cpu_state_t state, instr_t instr) {\
    static const rv_dispatch_t dispatch[] = {\
      __VA_ARGS__\
    };\
    [[gnu::musttail]] return dispatch[\
      GET_##field(instr)](\
      state,\
      instr);\
    }


[[gnu::always_inline]] static inline rv_i_imm_t extr_i_imm (rv_imm_t imm) {
  return (rv_i_imm_t){.val = (int32_t)(imm.raw << 20) >> 20};
}

void rv_32i_dispatcher(cpu_state_t state, instr_t instr);
