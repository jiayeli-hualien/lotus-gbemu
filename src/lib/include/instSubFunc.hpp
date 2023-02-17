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
#include "Iinstruction.hpp"
#include "instruction.hpp"

// types for fetcher / factory
// TODO: fetcher / decoder

namespace LOTUSGB {

#define SUB_FUNC_DECLARE(x) \
struct x {\
SUB_FUNC_OP_RET SUB_FUNC_OP;\
};

#define SUB_FUNC_IMPL(x) SUB_FUNC_OP_RET x::SUB_FUNC_OP

SUB_FUNC_DECLARE(subFuncNOP);
SUB_FUNC_DECLARE(subFuncLDRR);
SUB_FUNC_DECLARE(subFuncMemRead);
SUB_FUNC_DECLARE(subFuncLDR_IMMD);

}

#endif // LOTUSGB_INST_SUB_FUNC_HPP