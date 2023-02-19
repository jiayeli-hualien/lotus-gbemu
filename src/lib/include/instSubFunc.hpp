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

// flows of instruction per memory-cycles
// TODO: codegen? how to handle function not implemented?
namespace LOTUSGB {

#define SUB_FUNC_DECLARE(x) \
struct x {\
SUB_FUNC_OP_RET SUB_FUNC_OP;\
};

#define SUB_FUNC_IMPL(x) SUB_FUNC_OP_RET x::SUB_FUNC_OP

using subFunMapType = std::unordered_map<std::string, Instruction::FUNC_TYPE>;
subFunMapType getSubFuncMap();

SUB_FUNC_DECLARE(subFuncNOP);
SUB_FUNC_DECLARE(subFuncLDRR);
SUB_FUNC_DECLARE(subFuncLD_R_MEMVAL);
// TODO: get the realy GB pipeline, is there a subFuncLD_A_MEMVAL?
SUB_FUNC_DECLARE(subFuncLD_A_MEMVAL);
SUB_FUNC_DECLARE(subFuncLD_R16_MEM16);
// TODO: check why LD SP, HL need 2 cycles
SUB_FUNC_DECLARE(subFuncLD_SP_HL);
// TODO: check why don't PUSH Reg16 start writing from the first cycle?
SUB_FUNC_DECLARE(subFuncPUSH_LD_A16_RR);
SUB_FUNC_DECLARE(subFuncPOP_LD_R16_MEM16);
SUB_FUNC_DECLARE(subFuncMemReadPC);
SUB_FUNC_DECLARE(subFuncMemReadIndirectHL);
SUB_FUNC_DECLARE(subFuncMemReadIndirectHLDec);
SUB_FUNC_DECLARE(subFuncMemReadIndirectHLInc);
SUB_FUNC_DECLARE(subFuncMemReadIndirectBC_DE);
SUB_FUNC_DECLARE(subFuncMemReadIndirectA16);
SUB_FUNC_DECLARE(subFuncReadHighC);
SUB_FUNC_DECLARE(subFuncMemWriteIndirectHL);
SUB_FUNC_DECLARE(subFuncMemWriteIndirectHLDec);
SUB_FUNC_DECLARE(subFuncMemWriteIndirectHLInc);
SUB_FUNC_DECLARE(subFuncMemWriteIndirectBC_DE);
SUB_FUNC_DECLARE(subFuncMemWriteIndirectA16);
SUB_FUNC_DECLARE(subFuncMemWriteSPA16LSB);
SUB_FUNC_DECLARE(subFuncMemWriteSPA16MSB);
SUB_FUNC_DECLARE(subFuncWriteHighC);
SUB_FUNC_DECLARE(subFuncReadA16LSB);
SUB_FUNC_DECLARE(subFuncReadA16MSB);
SUB_FUNC_DECLARE(subFuncPUSH_A16_MSB);
SUB_FUNC_DECLARE(subFuncPUSH_A16_LSB);
SUB_FUNC_DECLARE(subFuncPOP_LD_A16_LSB);
SUB_FUNC_DECLARE(subFuncPOP_LD_A16_MSB);

}

#endif // LOTUSGB_INST_SUB_FUNC_HPP