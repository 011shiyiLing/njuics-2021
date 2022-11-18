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

#include <isa.h>
#include <cpu/difftest.h>
#include <memory/vaddr.h>
#include "../local-include/reg.h"

extern const char* regs[];

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool res = difftest_check_reg("pc", cpu.pc, ref_r->pc, cpu.pc);

  for(int i=0;i<32;i++)
  {
    res = difftest_check_reg(regs[i], cpu.pc, ref_r->gpr[i], cpu.gpr[i]);
    if(!res) return res;
  }
  return true;
}

void isa_difftest_attach() {
}
