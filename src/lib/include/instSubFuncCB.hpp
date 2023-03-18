/*
References:
Gameboy CPU (LR35902) instruction set
https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html

Game Boy: Complete Technical Referenc
https://gekkio.fi/files/gb-docs/gbctr.pdf

Game BoyTM CPU Manual
http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
*/
#ifndef LOTUSGB_INST_SUB_FUNC_CB_HPP
#define LOTUSGB_INST_SUB_FUNC_CB_HPP
#include <string>
#include "Iinstruction.hpp"
#include "instruction.hpp"
#include "instSubFuncMacro.hpp"

namespace LOTUSGB {

subFunMapType getSubFuncMapCB();

SUB_FUNC_DECLARE(RLC);
// TODO: read official doc of the CPU
// TODO: split one mem cycle to reg/mem
SUB_FUNC_DECLARE(RLC_MEMVAL_MemWriteIndirectHL);
SUB_FUNC_DECLARE(RRC);
SUB_FUNC_DECLARE(RRC_MEMVAL_MemWriteIndirectHL);
SUB_FUNC_DECLARE(RL);
SUB_FUNC_DECLARE(RL_MEMVAL_MemWriteIndirectHL);
SUB_FUNC_DECLARE(RR);
SUB_FUNC_DECLARE(RR_MEMVAL_MemWriteIndirectHL);
SUB_FUNC_DECLARE(SLA);
SUB_FUNC_DECLARE(SLA_MEMVAL_MemWriteIndirectHL);
SUB_FUNC_DECLARE(SRA);
SUB_FUNC_DECLARE(SRA_MEMVAL_MemWriteIndirectHL);
SUB_FUNC_DECLARE(SWAP);
SUB_FUNC_DECLARE(SWAP_MEMVAL_MemWriteIndirectHL);
}

#endif // LOTUSGB_INST_SUB_FUNC_CB_HPP
