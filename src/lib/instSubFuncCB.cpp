#include <iostream>
#include <cstddef>
#include <cstdint>
#include "include/instSubFuncCB.hpp"
#include "include/instSubFuncCommon.hpp"
#include "include/instruction.hpp"
#include "include/instSubFuncMacro.hpp"
#include "include/gb_reg.hpp"
#include "include/instSubFuncInlineFunc.hpp"

namespace LOTUSGB {

subFunMapType getSubFuncMapCB() {
    subFunMapType map;
    MAP_ENTRY(NOP);
    MAP_ENTRY(MemReadIndirectHL);
    MAP_ENTRY(RLC);
    MAP_ENTRY(RLC_MEMVAL_MemWriteIndirectHL);
    MAP_ENTRY(RRC);
    MAP_ENTRY(RRC_MEMVAL_MemWriteIndirectHL);
    return map;
}

static void _RLC(uint8_t *value, GB_Flag &flag) {
    int result = *value;
    flag.N = false;
    flag.H = false;
    flag.C = (1<<7)&result;
    result = (result << 1) | (flag.C ? 1 : 0);
    flag.Z = !result;
    *value = GET_LSB(result);
}

static inline void _CB_MemWriteIndirectHL(SUB_FUNC_PARAMS) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL();
}

SUB_FUNC_IMPL(RLC) {
    auto reg = getRegRHS(pInstState->opcode, pReg);
    if (!reg)
        return;
    GB_Flag flag = {};
    _RLC(reg, flag);
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(RLC_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = {};
    _RLC(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}

static void _RRC(uint8_t *value, GB_Flag &flag) {
    int result = *value;
    flag.N = false;
    flag.H = false;
    flag.C = (1)&result;
    result = (result) | (flag.C ? 1<<8 : 0);
    result >>= 1;
    flag.Z = !result;
    *value = GET_LSB(result);
}

SUB_FUNC_IMPL(RRC) {
    auto reg = getRegRHS(pInstState->opcode, pReg);
    if (!reg)
        return;
    GB_Flag flag = {};
    _RRC(reg, flag);
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(RRC_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = {};
    _RRC(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}

}