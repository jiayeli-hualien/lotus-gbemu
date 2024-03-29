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
    MAP_ENTRY(RL);
    MAP_ENTRY(RL_MEMVAL_MemWriteIndirectHL);
    MAP_ENTRY(RR);
    MAP_ENTRY(RR_MEMVAL_MemWriteIndirectHL);
    MAP_ENTRY(SLA);
    MAP_ENTRY(SLA_MEMVAL_MemWriteIndirectHL);
    MAP_ENTRY(SRA);
    MAP_ENTRY(SRA_MEMVAL_MemWriteIndirectHL);
    MAP_ENTRY(SWAP);
    MAP_ENTRY(SWAP_MEMVAL_MemWriteIndirectHL);
    MAP_ENTRY(SRL);
    MAP_ENTRY(SRL_MEMVAL_MemWriteIndirectHL);
    MAP_ENTRY(BIT);
    MAP_ENTRY(BIT_MEMVAL);
    MAP_ENTRY(RES);
    MAP_ENTRY(RES_MEMVAL_MemWriteIndirectHL);
    MAP_ENTRY(SET);
    MAP_ENTRY(SET_MEMVAL_MemWriteIndirectHL);
    return map;
}

static inline void _RLC(uint8_t *value, GB_Flag &flag) {
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
    if (auto reg = getRegRHS(pInstState->opcode, pReg)) {
        GB_Flag flag = {};
        _RLC(reg, flag);
        pReg->setFlag(flag);
    }
}

SUB_FUNC_IMPL(RLC_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = {};
    _RLC(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}

static inline void _RRC(uint8_t *value, GB_Flag &flag) {
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
    if (auto reg = getRegRHS(pInstState->opcode, pReg)) {
        GB_Flag flag = {};
        _RRC(reg, flag);
        pReg->setFlag(flag);
    }
}

SUB_FUNC_IMPL(RRC_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = {};
    _RRC(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}

static inline void _RL(uint8_t *value, GB_Flag &flag) {
    int result = *value;
    flag.N = false;
    flag.H = false;
    result = (result << 1) | (flag.C ? 1 : 0);
    flag.C = (1<<8)&result;
    flag.Z = !result;
    *value = GET_LSB(result);
}

SUB_FUNC_IMPL(RL) {
    if (auto reg = getRegRHS(pInstState->opcode, pReg)) {
        GB_Flag flag = pReg->getFlag();
        _RL(reg, flag);
        pReg->setFlag(flag);
    }
}

SUB_FUNC_IMPL(RL_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = pReg->getFlag();
    _RL(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}

static inline void _RR(uint8_t *value, GB_Flag &flag) {
    int result = *value;
    flag.N = false;
    flag.H = false;
    result = (result) | (flag.C ? 1<<8 : 0);
    flag.C = (1)&result;
    result >>= 1;
    flag.Z = !result;
    *value = GET_LSB(result);
}

SUB_FUNC_IMPL(RR) {
    if (auto reg = getRegRHS(pInstState->opcode, pReg)) {
        GB_Flag flag = pReg->getFlag();
        _RR(reg, flag);
        pReg->setFlag(flag);
    }
}

SUB_FUNC_IMPL(RR_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = pReg->getFlag();
    _RR(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}

static inline void _SLA(uint8_t *value, GB_Flag &flag) {
    int result = *value;
    flag.N = false;
    flag.H = false;
    flag.C = (1<<7)&result;
    flag.Z = !(result <<= 1);
    *value = GET_LSB(result);
}

SUB_FUNC_IMPL(SLA) {
    if (auto reg = getRegRHS(pInstState->opcode, pReg)) {
        GB_Flag flag = pReg->getFlag();
        _SLA(reg, flag);
        pReg->setFlag(flag);
    }
}

SUB_FUNC_IMPL(SLA_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = pReg->getFlag();
    _SLA(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}

static inline void _SRA(uint8_t *value, GB_Flag &flag) {
    int result = *value;
    flag.N = false;
    flag.H = false;
    flag.C = (1)&result;
    // keep MSB
    const int MSB = 1<<7;
    result = (result >> 1) | (result & MSB);
    flag.Z = !(result);
    *value = GET_LSB(result);
}

SUB_FUNC_IMPL(SRA) {
    if (auto reg = getRegRHS(pInstState->opcode, pReg)) {
        GB_Flag flag = pReg->getFlag();
        _SRA(reg, flag);
        pReg->setFlag(flag);
    }
}

SUB_FUNC_IMPL(SRA_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = pReg->getFlag();
    _SRA(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}

static inline void _SWAP(uint8_t *value, GB_Flag &flag) {
    int result = *value;
    flag.N = false;
    flag.H = false;
    flag.C = false;
    result = (result >> 4) | (result << 4);
    flag.Z = !(result);
    *value = GET_LSB(result);
}

SUB_FUNC_IMPL(SWAP) {
    if (auto reg = getRegRHS(pInstState->opcode, pReg)) {
        GB_Flag flag = {};
        _SWAP(reg, flag);
        pReg->setFlag(flag);
    }
}

SUB_FUNC_IMPL(SWAP_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = {};
    _SWAP(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}


static inline void _SRL(uint8_t *value, GB_Flag &flag) {
    int result = *value;
    flag.N = false;
    flag.H = false;
    flag.C = (1)&result;
    flag.Z = !(result >>= 1);
    *value = GET_LSB(result);
}

SUB_FUNC_IMPL(SRL) {
    if (auto reg = getRegRHS(pInstState->opcode, pReg)) {
        GB_Flag flag = {};
        _SRL(reg, flag);
        pReg->setFlag(flag);
    }
}

SUB_FUNC_IMPL(SRL_MEMVAL_MemWriteIndirectHL) {
    GB_Flag flag = {};
    _SRL(&pInstState->memValue, flag);
    pReg->setFlag(flag);

    _CB_MemWriteIndirectHL(SUB_FUNC_ARGS);
}

static inline int getLHS_Idx(const uint8_t &opcode) {
    return ((opcode >> 3) & 0x7);
}

static inline void _BIT(uint8_t *value, GB_Flag &flag, const uint8_t &opcode) {
    const int pos = getLHS_Idx(opcode);
    flag.N = false;
    flag.H = true;
    flag.Z = !((1<<pos) & (*value));
}

SUB_FUNC_IMPL(BIT) {
    if (auto reg = getRegRHS(pInstState->opcode, pReg)) {
        GB_Flag flag = pReg->getFlag();
        _BIT(reg, flag, pInstState->opcode);
        pReg->setFlag(flag);
    }
}

SUB_FUNC_IMPL(BIT_MEMVAL) {
    GB_Flag flag = pReg->getFlag();
    _BIT(&pInstState->memValue, flag, pInstState->opcode);
    pReg->setFlag(flag);

    // TODO: check spec & memcycle, why does BIT (HL) need 16 cycles?
    pInstState->memMode = MEM_MODE_SLEEP;
}

static inline void _RES(uint8_t *value, const uint8_t &opcode) {
    // reset means clear bit
    const int pos = getLHS_Idx(opcode);
    const int MASK = ~(1<<pos);
    *value &= MASK;
}

SUB_FUNC_IMPL(RES) {
    if (auto reg = getRegRHS(pInstState->opcode, pReg))
        _RES(reg, pInstState->opcode);
}

SUB_FUNC_IMPL(RES_MEMVAL_MemWriteIndirectHL) {
    _RES(&pInstState->memValue, pInstState->opcode);

    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL();
}

static inline void _SET(uint8_t *value, const uint8_t &opcode) {
    const int pos = getLHS_Idx(opcode);
    const int MASK = (1<<pos);
    *value |= MASK;
}

SUB_FUNC_IMPL(SET) {
    if (auto reg = getRegRHS(pInstState->opcode, pReg))
        _SET(reg, pInstState->opcode);
}

SUB_FUNC_IMPL(SET_MEMVAL_MemWriteIndirectHL) {
    _SET(&pInstState->memValue, pInstState->opcode);

    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL();
}

}
