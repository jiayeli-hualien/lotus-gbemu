#include <iostream>
#include <cstddef>
#include <cstdint>
#include "include/instSubFunc.hpp"
#include "include/instruction.hpp"
#include "include/gb_reg.hpp"

namespace LOTUSGB {

static inline uint8_t* _getReg(const int &number, Reg *pReg) {
    switch (number) {
        case 0: return &pReg->getRefB();
        case 1: return &pReg->getRefC();
        case 2: return &pReg->getRefD();
        case 3: return &pReg->getRefE();
        case 4: return &pReg->getRefH();
        case 5: return &pReg->getRefL();
        // 6: HL is handled in 16bit LD instruction
        case 7: return &pReg->getRefA();
        default:
            return nullptr;
    }
}

static uint8_t* getRegLHS(const uint8_t &op, Reg *pReg) {
    constexpr uint8_t MASK = 0x3F;
    const int num = ((op&MASK)>>3U);
    auto ret = _getReg(num, pReg);
    if (!ret)
        std::cerr << "invalid OP code, reg not support" << (int)op << std::endl;
    return ret;
}

static uint8_t* getRegRHS(const uint8_t &op, Reg *pReg) {
    constexpr uint8_t MASK = 0x07;
    const int num = (op & MASK);
    auto ret = _getReg(num, pReg);
    return ret;
}


subFunMapType getSubFuncMap() {
    subFunMapType map;
    // TODO: codegen?
#define MAP_ENTRY(name) (map[#name]=name())
    MAP_ENTRY(subFuncNOP);
    MAP_ENTRY(subFuncLDRR);
    MAP_ENTRY(subFuncLD_R_MEMVAL);
    MAP_ENTRY(subFuncLD_A_MEMVAL);
    MAP_ENTRY(subFuncLD_R16_MEM16);
    MAP_ENTRY(subFuncLD_SP_HL);
    MAP_ENTRY(subFuncPUSH_LD_A16_RR);
    MAP_ENTRY(subFuncPOP_LD_R16_MEM16);
    MAP_ENTRY(subFuncMemReadPC);
    MAP_ENTRY(subFuncMemReadIndirectHL);
    MAP_ENTRY(subFuncMemReadIndirectHLDec);
    MAP_ENTRY(subFuncMemReadIndirectHLInc);
    MAP_ENTRY(subFuncMemReadIndirectBC_DE);
    MAP_ENTRY(subFuncMemReadIndirectA16);
    MAP_ENTRY(subFuncReadHighC);
    MAP_ENTRY(subFuncMemWriteIndirectHL);
    MAP_ENTRY(subFuncMemWriteIndirectHLDec);
    MAP_ENTRY(subFuncMemWriteIndirectHLInc);
    MAP_ENTRY(subFuncMemWriteIndirectBC_DE);
    MAP_ENTRY(subFuncMemWriteIndirectA16);
    MAP_ENTRY(subFuncMemWriteSPA16LSB);
    MAP_ENTRY(subFuncMemWriteSPA16MSB);
    MAP_ENTRY(subFuncWriteHighC);
    MAP_ENTRY(subFuncReadA16LSB);
    MAP_ENTRY(subFuncReadA16MSB);
    MAP_ENTRY(subFuncPUSH_A16_MSB);
    MAP_ENTRY(subFuncPUSH_A16_LSB);
    MAP_ENTRY(subFuncPOP_LD_A16_LSB);
    MAP_ENTRY(subFuncPOP_LD_A16_MSB);
    MAP_ENTRY(subFuncAddA_R);
    MAP_ENTRY(subFuncAddA_R_Carry);
    MAP_ENTRY(subFuncAddA_MEMVAL);
    MAP_ENTRY(subFuncAddA_MEMVAL_Carry);
    MAP_ENTRY(subFuncSubA_R);
    MAP_ENTRY(subFuncSubA_MEMVAL);
    MAP_ENTRY(subFuncSubA_R_Carry);
    MAP_ENTRY(subFuncSubA_MEMVAL_Carry);
    MAP_ENTRY(subFuncCmpA_R);
    MAP_ENTRY(subFuncCmpA_MEMVAL);
    MAP_ENTRY(subFuncIncR);
    MAP_ENTRY(subFuncMemIncWriteIndirectHL);
    MAP_ENTRY(subFuncDecR);
    MAP_ENTRY(subFuncMemDecWriteIndirectHL);
    MAP_ENTRY(subFuncAndA_R);
    MAP_ENTRY(subFuncAndA_MEMVAL);

    return map;
}


SUB_FUNC_IMPL(subFuncNOP){
    return;
}

SUB_FUNC_IMPL(subFuncLDRR) {
    // TODO: error handling
    const uint8_t &op = pInstState->opcode;
    uint8_t *lhs = getRegLHS(op, pReg);
    uint8_t *rhs = getRegRHS(op, pReg);
    if (lhs && rhs)
        *lhs = *rhs;
}

SUB_FUNC_IMPL(subFuncLD_SP_HL) {
    pInstState->memMode = MEM_MODE_SLEEP;
    pReg->getRefSP() = pReg->getRefHL();
}

static uint16_t& _getPushPopReg16(uint8_t op, Reg *pReg) {
    switch((op >> 4) & 0x03) {
        case 0: return pReg->getRefBC();
        case 1: return pReg->getRefDE();
        case 2: return pReg->getRefHL();
        case 3:
        default:
            return pReg->getRefAF();
    }
}

SUB_FUNC_IMPL(subFuncPUSH_LD_A16_RR) {
    pInstState->memMode = MEM_MODE_SLEEP;
    pInstState->a16Addr = _getPushPopReg16(pInstState->opcode, pReg);
}

SUB_FUNC_IMPL(subFuncPOP_LD_R16_MEM16) {
    pInstState->memMode = MEM_MODE_NONE;
    _getPushPopReg16(pInstState->opcode, pReg) = pInstState->a16Addr;
    // TODO: refine F's behavior
    // non-flags always zero
    pReg->getRefF() = pReg->getRefF() & 0xF0;
}

SUB_FUNC_IMPL(subFuncMemReadPC) {
    // notify CPU to read immediate value
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pReg->getRefPC()++;
}

static uint16_t& _getIndreictRegBC_DE(InstState *pInstState, Reg *pReg) {
    switch(pInstState->opcode) {
        case 0x02:
        case 0x0A:
            return pReg->getRefBC();
        case 0x12:
        case 0x1A:
        default:
            return pReg->getRefDE();
    }
}

static uint16_t& _getCommonReg16(uint8_t opcode, Reg *pReg) {
    switch((opcode >> 4)&0x3) {
        case 0: return pReg->getRefBC();
        case 1: return pReg->getRefDE();
        case 2: return pReg->getRefHL();
        case 3:
        default:
            return pReg->getRefSP();
    }
}

SUB_FUNC_IMPL(subFuncMemReadIndirectHL) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pReg->getRefHL();
}

SUB_FUNC_IMPL(subFuncMemReadIndirectHLDec) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pReg->getRefHL()--;
}

SUB_FUNC_IMPL(subFuncMemReadIndirectHLInc) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pReg->getRefHL()++;
}

SUB_FUNC_IMPL(subFuncMemReadIndirectBC_DE) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = _getIndreictRegBC_DE(pInstState, pReg);
}

SUB_FUNC_IMPL(subFuncMemReadIndirectA16) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pInstState->a16Addr;
}

SUB_FUNC_IMPL(subFuncReadHighC) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = 0xF0 | pReg->getRefC();
}

SUB_FUNC_IMPL(subFuncReadA16LSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_LSB;
    pInstState->memAddr = pReg->getRefPC()++;
}

SUB_FUNC_IMPL(subFuncReadA16MSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_MSB;
    pInstState->memAddr = pReg->getRefPC()++;
}

SUB_FUNC_IMPL(subFuncMemWriteIndirectHL) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL();
    uint8_t *rhs = getRegRHS(pInstState->opcode, pReg);
    if (rhs)
        pInstState->memValue = *rhs;
}

SUB_FUNC_IMPL(subFuncMemWriteIndirectHLDec) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL()--;
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(subFuncMemWriteIndirectHLInc) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL()++;
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(subFuncMemWriteIndirectBC_DE) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = _getIndreictRegBC_DE(pInstState, pReg);
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(subFuncMemWriteIndirectA16) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pInstState->a16Addr;
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(subFuncMemWriteSPA16LSB) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pInstState->a16Addr;
    pInstState->memValue = (pReg->getRefSP()) & 0xFF;
}

SUB_FUNC_IMPL(subFuncMemWriteSPA16MSB) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pInstState->a16Addr + 1;
    pInstState->memValue = (pReg->getRefSP()>>8) & 0xFF;
}

SUB_FUNC_IMPL(subFuncWriteHighC) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = HIGH_MEM_ADDR | pReg->getRefC();
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(subFuncLD_R_MEMVAL) {
    uint8_t *lhs = getRegLHS(pInstState->opcode, pReg);
    if (lhs)
        *lhs = pInstState->memValue;
}

SUB_FUNC_IMPL(subFuncLD_A_MEMVAL) {
    pReg->getRefA() = pInstState->memValue;
}

SUB_FUNC_IMPL(subFuncLD_R16_MEM16) {
    _getCommonReg16(pInstState->opcode, pReg) = pInstState->a16Addr;
}

SUB_FUNC_IMPL(subFuncPUSH_A16_MSB) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = --pReg->getRefSP();
    pInstState->memValue = (pInstState->a16Addr>>8)&0xFF;
}

SUB_FUNC_IMPL(subFuncPUSH_A16_LSB) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = --pReg->getRefSP();
    pInstState->memValue = (pInstState->a16Addr)&0xFF;
}

SUB_FUNC_IMPL(subFuncPOP_LD_A16_LSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_LSB;
    pInstState->memAddr = pReg->getRefSP()++;
}

SUB_FUNC_IMPL(subFuncPOP_LD_A16_MSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_MSB;
    pInstState->memAddr = pReg->getRefSP()++;
}

// int for leverage int promotion
static constexpr int BYTE_MASK = 0xFF;
static constexpr int HALF_MASK = 0xF;
#define GET_LSB(x) ((uint8_t)((x) & BYTE_MASK))
#define GET_LSB_INT(x) ((x) & BYTE_MASK)
#define GET_HALF_INT(x) ((x) & HALF_MASK)
static constexpr int HALF_UPPER_BOUND = 0xF;
static constexpr int BYTE_UPPER_BOUND = 0xFF;
#define HALF_OP_HELPER(x, op, y) (GET_HALF_INT((x)) op GET_HALF_INT((y)))

SUB_FUNC_IMPL(subFuncAddA_R) {
    uint8_t *rhs = getRegRHS(pInstState->opcode, pReg);
    if (!rhs)
        return;
    const int result = pReg->getRefA() + *rhs;
    GB_Flag flag = {};

    flag.Z = (!GET_LSB(result));
    flag.N = false;
    flag.H = (HALF_OP_HELPER(pReg->getRefA(), +, *rhs) >
             HALF_UPPER_BOUND);
    flag.C = result > BYTE_UPPER_BOUND;
    pReg->setFlag(flag);
    pReg->getRefA() = GET_LSB(result);
}

SUB_FUNC_IMPL(subFuncAddA_MEMVAL) {
    const int result = pReg->getRefA() + pInstState->memValue;
    GB_Flag flag = {};

    flag.Z = (!GET_LSB(result));
    flag.N = false;
    flag.H = (HALF_OP_HELPER(pReg->getRefA(), +, pInstState->memValue) >
             HALF_UPPER_BOUND);
    flag.C = result > BYTE_UPPER_BOUND;
    pReg->setFlag(flag);
    pReg->getRefA() = GET_LSB(result);
}

// TODO: refactor?
SUB_FUNC_IMPL(subFuncAddA_R_Carry) {
    uint8_t *rhs = getRegRHS(pInstState->opcode, pReg);
    if (!rhs)
        return;
    GB_Flag flag = {};
    pReg->getFlag(flag);

    const int vc = flag.C ? 1 : 0;
    const int result = pReg->getRefA() + *rhs + vc;
    flag.Z = (!GET_LSB(result));
    flag.N = false;
    flag.H = (HALF_OP_HELPER(pReg->getRefA(), +, *rhs) + vc >
             HALF_UPPER_BOUND);
    flag.C = result > BYTE_UPPER_BOUND;
    pReg->setFlag(flag);
    pReg->getRefA() = GET_LSB(result);
}

SUB_FUNC_IMPL(subFuncAddA_MEMVAL_Carry) {
    GB_Flag flag = {};
    pReg->getFlag(flag);

    const int vc = flag.C ? 1 : 0;
    const int result = pReg->getRefA() + pInstState->memValue + vc;
    flag.Z = (!GET_LSB(result));
    flag.N = false;
    flag.H = (HALF_OP_HELPER(pReg->getRefA(), +, pInstState->memValue) + vc >
             HALF_UPPER_BOUND);
    flag.C = result > BYTE_UPPER_BOUND;
    pReg->setFlag(flag);
    pReg->getRefA() = GET_LSB(result);
}

struct SubArgs {
    int rhs = 0U;
    bool useCarry = false;
    bool updateA = true;
    SubArgs() {};
    SubArgs(int iRhs, bool iUseCarry, bool iUpdateA)
    : rhs(iRhs), useCarry(iUseCarry), updateA(iUpdateA) {};
};

static inline void _subCommon(const SubArgs &args, SUB_FUNC_PARAMS) {
    GB_Flag flag = {};
    pReg->getFlag(flag);

    const int vc = args.useCarry && flag.C ? 1 : 0;
    const int result = pReg->getRefA() - (args.rhs + vc);
    flag.Z = (!GET_LSB(result));
    flag.N = true;
    flag.H = GET_HALF_INT(args.rhs) + vc > GET_HALF_INT(pReg->getRefA());
    flag.C = result < 0;
    pReg->setFlag(flag);
    pReg->getRefA() = (args.updateA ? GET_LSB(result) : pReg->getRefA());
}

SUB_FUNC_IMPL(subFuncSubA_R) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _subCommon(SubArgs(*rhs, false, true), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(subFuncSubA_MEMVAL) {
    _subCommon(SubArgs(pInstState->memValue, false, true), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(subFuncSubA_R_Carry) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _subCommon(SubArgs(*rhs, true, true), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(subFuncSubA_MEMVAL_Carry) {
    _subCommon(SubArgs(pInstState->memValue, true, true), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(subFuncCmpA_R) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _subCommon(SubArgs(*rhs, false, false), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(subFuncCmpA_MEMVAL) {
    _subCommon(SubArgs(pInstState->memValue, false, false), SUB_FUNC_ARGS);
}

static inline void _incUint8Common(uint8_t *lhs, SUB_FUNC_PARAMS) {
    GB_Flag flag = {};
    pReg->getFlag(flag);
    const uint8_t tmp = (*lhs)++;
    flag.Z = (!(*lhs));
    flag.N = false;
    flag.H = (HALF_OP_HELPER(tmp, +, 1) > HALF_UPPER_BOUND);
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(subFuncIncR) {
    if (uint8_t *lhs = getRegLHS(pInstState->opcode, pReg))
        _incUint8Common(lhs, SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(subFuncMemIncWriteIndirectHL) {
    _incUint8Common(&pInstState->memValue, SUB_FUNC_ARGS);
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL();
}

static inline void _decUint8Common(uint8_t *lhs, SUB_FUNC_PARAMS) {
    GB_Flag flag = {};
    pReg->getFlag(flag);
    const uint8_t tmp = (*lhs)--;
    flag.Z = (!(*lhs));
    flag.N = true;
    flag.H = (!GET_HALF_INT(tmp)); // lower 4bit < 1
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(subFuncDecR) {
    if (uint8_t *lhs = getRegLHS(pInstState->opcode, pReg))
        _decUint8Common(lhs, SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(subFuncMemDecWriteIndirectHL) {
    _decUint8Common(&pInstState->memValue, SUB_FUNC_ARGS);
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL();
}


static inline void _andUint8Common(uint8_t *rhs, SUB_FUNC_PARAMS) {
    pReg->getRefA() = GET_LSB(pReg->getRefA() & (*rhs));
    GB_Flag flag;
    flag.Z = (!pReg->getRefA());
    flag.N = false;
    flag.H = true;
    flag.C = false;
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(subFuncAndA_R) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _andUint8Common(rhs, SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(subFuncAndA_MEMVAL) {
    _andUint8Common(&pInstState->memValue, SUB_FUNC_ARGS);
}

}
