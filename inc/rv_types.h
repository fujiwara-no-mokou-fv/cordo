#pragma once
#include<stdint.h>



typedef struct {
  uint32_t raw;
} instr_t;

typedef struct {
  uint32_t raw;
} rv_imm_t;

typedef struct {
  int32_t val;
} rv_i_imm_t;

typedef uint32_t field_t;
