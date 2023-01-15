/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#ifndef __ISA_RISCV32_H__
#define __ISA_RISCV32_H__

#include <common.h>

typedef struct
{
  word_t data;
  uint32_t MIE : 1;
  uint32_t MPIE : 1;
}CSR;

typedef struct {
  word_t gpr[32];
  word_t csr[5];
  CSR mstatus;
  vaddr_t pc;
  bool INTR;
} riscv32_CPU_state;


// decode
typedef struct {
  union {
    uint32_t val;
  } inst;
} riscv32_ISADecodeInfo;

//检查当前系统状态下对内存区间为[vaddr, vaddr + len), 类型为type的访问是否需要经过地址转换.
//#define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)
#define isa_mmu_check(vaddr, len, type) (cpu.csr[4] & (1UL << 31) ? MMU_TRANSLATE : MMU_DIRECT)
// csr[4] : satp register
#endif
