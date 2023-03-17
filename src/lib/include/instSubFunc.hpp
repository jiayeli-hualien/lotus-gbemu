/*
References:
Gameboy CPU (LR35902) instruction set
https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html

Game Boy: Complete Technical Referenc
https://gekkio.fi/files/gb-docs/gbctr.pdf

Game BoyTM CPU Manual
http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
*/
#ifndef LOTUSGB_INST_SUB_FUNC_HPP
#define LOTUSGB_INST_SUB_FUNC_HPP
#include <string>
#include <unordered_map>
#include "Iinstruction.hpp"
#include "instruction.hpp"
#include "instSubFuncMacro.hpp"

// flows of instruction per memory-cycles
// TODO: codegen? how to handle function not implemented?
namespace LOTUSGB {

using subFunMapType = std::unordered_map<std::string, Instruction::FUNC_TYPE>;
subFunMapType getSubFuncMap();

SUB_FUNC_DECLARE(NOP);
SUB_FUNC_DECLARE(LDRR);
SUB_FUNC_DECLARE(LD_R_MEMVAL);
// TODO: get the realy GB pipeline, is there a LD_A_MEMVAL?
SUB_FUNC_DECLARE(LD_A_MEMVAL);
SUB_FUNC_DECLARE(LD_R16_MEM16);
// TODO: check why LD SP, HL need 2 cycles
SUB_FUNC_DECLARE(LD_SP_HL);
// TODO: check why don't PUSH Reg16 start writing from the first cycle?
SUB_FUNC_DECLARE(PUSH_LD_A16_RR);
SUB_FUNC_DECLARE(POP_LD_R16_MEM16);
SUB_FUNC_DECLARE(MemReadPC);
SUB_FUNC_DECLARE(MemReadIndirectHL);
SUB_FUNC_DECLARE(MemReadIndirectHLDec);
SUB_FUNC_DECLARE(MemReadIndirectHLInc);
SUB_FUNC_DECLARE(MemReadIndirectBC_DE);
SUB_FUNC_DECLARE(MemReadIndirectA16);
SUB_FUNC_DECLARE(ReadHighC);
SUB_FUNC_DECLARE(MemWriteIndirectHL);
SUB_FUNC_DECLARE(MemWriteIndirectHLDec);
SUB_FUNC_DECLARE(MemWriteIndirectHLInc);
SUB_FUNC_DECLARE(MemWriteIndirectBC_DE);
SUB_FUNC_DECLARE(MemWriteIndirectA16);
SUB_FUNC_DECLARE(MemWriteSPA16LSB);
SUB_FUNC_DECLARE(MemWriteSPA16MSB);
SUB_FUNC_DECLARE(WriteHighC);
SUB_FUNC_DECLARE(ReadA16LSB);
SUB_FUNC_DECLARE(ReadA16MSB);
SUB_FUNC_DECLARE(PUSH_A16_MSB);
SUB_FUNC_DECLARE(PUSH_A16_LSB);
SUB_FUNC_DECLARE(POP_LD_A16_LSB);
SUB_FUNC_DECLARE(POP_LD_A16_MSB);
SUB_FUNC_DECLARE(AddA_R);
SUB_FUNC_DECLARE(AddA_MEMVAL);
SUB_FUNC_DECLARE(AddA_R_Carry);
SUB_FUNC_DECLARE(AddA_MEMVAL_Carry)
SUB_FUNC_DECLARE(SubA_R);
SUB_FUNC_DECLARE(SubA_MEMVAL);
SUB_FUNC_DECLARE(SubA_R_Carry);
SUB_FUNC_DECLARE(SubA_MEMVAL_Carry);
SUB_FUNC_DECLARE(CmpA_R);
SUB_FUNC_DECLARE(CmpA_MEMVAL);
SUB_FUNC_DECLARE(IncR);
SUB_FUNC_DECLARE(MemIncWriteIndirectHL);
SUB_FUNC_DECLARE(DecR);
SUB_FUNC_DECLARE(MemDecWriteIndirectHL);
SUB_FUNC_DECLARE(AndA_R);
SUB_FUNC_DECLARE(AndA_MEMVAL);
SUB_FUNC_DECLARE(OrA_R);
SUB_FUNC_DECLARE(OrA_MEMVAL);
SUB_FUNC_DECLARE(XorA_R);
SUB_FUNC_DECLARE(XorA_MEMVAL);
SUB_FUNC_DECLARE(CCF);
SUB_FUNC_DECLARE(SCF);
SUB_FUNC_DECLARE(DAA);
SUB_FUNC_DECLARE(CPL);
SUB_FUNC_DECLARE(LD_PC_MEM16);
SUB_FUNC_DECLARE(LD_PC_HL);
SUB_FUNC_DECLARE(CondLD_PC_MEM16);
SUB_FUNC_DECLARE(JR);
SUB_FUNC_DECLARE(CondJR);
SUB_FUNC_DECLARE(SwapPC_MEM16);
SUB_FUNC_DECLARE(CondSwapPC_MEM16);
// TODO: why GB need idle at this mem clock?
// TODO: let SP value correct in every clock.
SUB_FUNC_DECLARE(Bubble);
SUB_FUNC_DECLARE(CondPOP_LD_A16_LSB);
SUB_FUNC_DECLARE(LD_PC_MEM16_RETI);
SUB_FUNC_DECLARE(LD_MEM16_PC);
SUB_FUNC_DECLARE(RST);
SUB_FUNC_DECLARE(Halt);
SUB_FUNC_DECLARE(Stop);
SUB_FUNC_DECLARE(StopWorkaround);
SUB_FUNC_DECLARE(DI);
SUB_FUNC_DECLARE(EI);
// TODO: get correct pipeline spec of 16bit inst
// TODO: implment all add & sub only by 8bit adder?
SUB_FUNC_DECLARE(IncRR);
SUB_FUNC_DECLARE(DecRR);
SUB_FUNC_DECLARE(Add_HL_RR);
SUB_FUNC_DECLARE(Add_SP_R8);
SUB_FUNC_DECLARE(Add_HL_SP_R8);
SUB_FUNC_DECLARE(RLCA);
SUB_FUNC_DECLARE(RLA);
SUB_FUNC_DECLARE(RRCA);
SUB_FUNC_DECLARE(RRA);

}

#endif // LOTUSGB_INST_SUB_FUNC_HPP