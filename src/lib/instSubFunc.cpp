#include <iostream>
#include <cstddef>
#include <cstdint>
#include "include/instSubFunc.hpp"
#include "include/instruction.hpp"
#include "include/instSubFuncMacro.hpp"
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
    MAP_ENTRY(NOP);
    MAP_ENTRY(LDRR);
    MAP_ENTRY(LD_R_MEMVAL);
    MAP_ENTRY(LD_A_MEMVAL);
    MAP_ENTRY(LD_R16_MEM16);
    MAP_ENTRY(LD_SP_HL);
    MAP_ENTRY(PUSH_LD_A16_RR);
    MAP_ENTRY(POP_LD_R16_MEM16);
    MAP_ENTRY(MemReadPC);
    MAP_ENTRY(MemReadIndirectHL);
    MAP_ENTRY(MemReadIndirectHLDec);
    MAP_ENTRY(MemReadIndirectHLInc);
    MAP_ENTRY(MemReadIndirectBC_DE);
    MAP_ENTRY(MemReadIndirectA16);
    MAP_ENTRY(ReadHighC);
    MAP_ENTRY(MemWriteIndirectHL);
    MAP_ENTRY(MemWriteIndirectHLDec);
    MAP_ENTRY(MemWriteIndirectHLInc);
    MAP_ENTRY(MemWriteIndirectBC_DE);
    MAP_ENTRY(MemWriteIndirectA16);
    MAP_ENTRY(MemWriteSPA16LSB);
    MAP_ENTRY(MemWriteSPA16MSB);
    MAP_ENTRY(WriteHighC);
    MAP_ENTRY(ReadA16LSB);
    MAP_ENTRY(ReadA16MSB);
    MAP_ENTRY(PUSH_A16_MSB);
    MAP_ENTRY(PUSH_A16_LSB);
    MAP_ENTRY(POP_LD_A16_LSB);
    MAP_ENTRY(POP_LD_A16_MSB);
    MAP_ENTRY(AddA_R);
    MAP_ENTRY(AddA_R_Carry);
    MAP_ENTRY(AddA_MEMVAL);
    MAP_ENTRY(AddA_MEMVAL_Carry);
    MAP_ENTRY(SubA_R);
    MAP_ENTRY(SubA_MEMVAL);
    MAP_ENTRY(SubA_R_Carry);
    MAP_ENTRY(SubA_MEMVAL_Carry);
    MAP_ENTRY(CmpA_R);
    MAP_ENTRY(CmpA_MEMVAL);
    MAP_ENTRY(IncR);
    MAP_ENTRY(MemIncWriteIndirectHL);
    MAP_ENTRY(DecR);
    MAP_ENTRY(MemDecWriteIndirectHL);
    MAP_ENTRY(AndA_R);
    MAP_ENTRY(AndA_MEMVAL);
    MAP_ENTRY(OrA_R);
    MAP_ENTRY(OrA_MEMVAL);
    MAP_ENTRY(XorA_R);
    MAP_ENTRY(XorA_MEMVAL);
    MAP_ENTRY(CCF);
    MAP_ENTRY(SCF);
    MAP_ENTRY(DAA);
    MAP_ENTRY(CPL);
    MAP_ENTRY(LD_PC_MEM16);
    MAP_ENTRY(LD_PC_HL);
    MAP_ENTRY(CondLD_PC_MEM16);
    MAP_ENTRY(JR);
    MAP_ENTRY(CondJR);
    MAP_ENTRY(SwapPC_MEM16);
    MAP_ENTRY(CondSwapPC_MEM16);
    MAP_ENTRY(Bubble);
    MAP_ENTRY(CondPOP_LD_A16_LSB);
    MAP_ENTRY(LD_PC_MEM16_RETI);
    MAP_ENTRY(LD_MEM16_PC);
    MAP_ENTRY(RST);
    MAP_ENTRY(Halt);
    MAP_ENTRY(Stop);
    MAP_ENTRY(StopWorkaround);
    MAP_ENTRY(DI);
    MAP_ENTRY(EI);
    MAP_ENTRY(IncRR);
    MAP_ENTRY(DecRR);
    MAP_ENTRY(Add_HL_RR);
    MAP_ENTRY(Add_SP_R8);
    MAP_ENTRY(Add_HL_SP_R8);
    MAP_ENTRY(RLCA);
    MAP_ENTRY(RLA);
    MAP_ENTRY(RRCA);
    MAP_ENTRY(RRA);

    return map;
}


SUB_FUNC_IMPL(NOP){
    return;
}

SUB_FUNC_IMPL(LDRR) {
    // TODO: error handling
    const uint8_t &op = pInstState->opcode;
    uint8_t *lhs = getRegLHS(op, pReg);
    uint8_t *rhs = getRegRHS(op, pReg);
    if (lhs && rhs)
        *lhs = *rhs;
}

SUB_FUNC_IMPL(LD_SP_HL) {
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

SUB_FUNC_IMPL(PUSH_LD_A16_RR) {
    pInstState->memMode = MEM_MODE_SLEEP;
    pInstState->a16Addr = _getPushPopReg16(pInstState->opcode, pReg);
}

SUB_FUNC_IMPL(POP_LD_R16_MEM16) {
    pInstState->memMode = MEM_MODE_FETCH;
    _getPushPopReg16(pInstState->opcode, pReg) = pInstState->a16Addr;
    // TODO: refine F's behavior
    // non-flags always zero
    pReg->getRefF() = pReg->getRefF() & 0xF0;
}

SUB_FUNC_IMPL(MemReadPC) {
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

SUB_FUNC_IMPL(MemReadIndirectHL) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pReg->getRefHL();
}

SUB_FUNC_IMPL(MemReadIndirectHLDec) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pReg->getRefHL()--;
}

SUB_FUNC_IMPL(MemReadIndirectHLInc) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pReg->getRefHL()++;
}

SUB_FUNC_IMPL(MemReadIndirectBC_DE) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = _getIndreictRegBC_DE(pInstState, pReg);
}

SUB_FUNC_IMPL(MemReadIndirectA16) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pInstState->a16Addr;
}

SUB_FUNC_IMPL(ReadHighC) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = 0xF0 | pReg->getRefC();
}

SUB_FUNC_IMPL(ReadA16LSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_LSB;
    pInstState->memAddr = pReg->getRefPC()++;
}

SUB_FUNC_IMPL(ReadA16MSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_MSB;
    pInstState->memAddr = pReg->getRefPC()++;
}

SUB_FUNC_IMPL(MemWriteIndirectHL) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL();
    uint8_t *rhs = getRegRHS(pInstState->opcode, pReg);
    if (rhs)
        pInstState->memValue = *rhs;
}

SUB_FUNC_IMPL(MemWriteIndirectHLDec) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL()--;
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(MemWriteIndirectHLInc) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pReg->getRefHL()++;
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(MemWriteIndirectBC_DE) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = _getIndreictRegBC_DE(pInstState, pReg);
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(MemWriteIndirectA16) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pInstState->a16Addr;
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(MemWriteSPA16LSB) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pInstState->a16Addr;
    pInstState->memValue = (pReg->getRefSP()) & 0xFF;
}

SUB_FUNC_IMPL(MemWriteSPA16MSB) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = pInstState->a16Addr + 1;
    pInstState->memValue = (pReg->getRefSP()>>8) & 0xFF;
}

SUB_FUNC_IMPL(WriteHighC) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = HIGH_MEM_ADDR | pReg->getRefC();
    pInstState->memValue = pReg->getRefA();
}

SUB_FUNC_IMPL(LD_R_MEMVAL) {
    uint8_t *lhs = getRegLHS(pInstState->opcode, pReg);
    if (lhs)
        *lhs = pInstState->memValue;
}

SUB_FUNC_IMPL(LD_A_MEMVAL) {
    pReg->getRefA() = pInstState->memValue;
}

SUB_FUNC_IMPL(LD_R16_MEM16) {
    _getCommonReg16(pInstState->opcode, pReg) = pInstState->a16Addr;
}

SUB_FUNC_IMPL(PUSH_A16_MSB) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = --pReg->getRefSP();
    pInstState->memValue = (pInstState->a16Addr>>8)&0xFF;
}

SUB_FUNC_IMPL(PUSH_A16_LSB) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = --pReg->getRefSP();
    pInstState->memValue = (pInstState->a16Addr)&0xFF;
}

SUB_FUNC_IMPL(POP_LD_A16_LSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_LSB;
    pInstState->memAddr = pReg->getRefSP()++;
}

SUB_FUNC_IMPL(POP_LD_A16_MSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_MSB;
    pInstState->memAddr = pReg->getRefSP()++;
}

SUB_FUNC_IMPL(AddA_R) {
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

SUB_FUNC_IMPL(AddA_MEMVAL) {
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
SUB_FUNC_IMPL(AddA_R_Carry) {
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

SUB_FUNC_IMPL(AddA_MEMVAL_Carry) {
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

SUB_FUNC_IMPL(SubA_R) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _subCommon(SubArgs(*rhs, false, true), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(SubA_MEMVAL) {
    _subCommon(SubArgs(pInstState->memValue, false, true), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(SubA_R_Carry) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _subCommon(SubArgs(*rhs, true, true), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(SubA_MEMVAL_Carry) {
    _subCommon(SubArgs(pInstState->memValue, true, true), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(CmpA_R) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _subCommon(SubArgs(*rhs, false, false), SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(CmpA_MEMVAL) {
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

SUB_FUNC_IMPL(IncR) {
    if (uint8_t *lhs = getRegLHS(pInstState->opcode, pReg))
        _incUint8Common(lhs, SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(MemIncWriteIndirectHL) {
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

SUB_FUNC_IMPL(DecR) {
    if (uint8_t *lhs = getRegLHS(pInstState->opcode, pReg))
        _decUint8Common(lhs, SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(MemDecWriteIndirectHL) {
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

SUB_FUNC_IMPL(AndA_R) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _andUint8Common(rhs, SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(AndA_MEMVAL) {
    _andUint8Common(&pInstState->memValue, SUB_FUNC_ARGS);
}


static inline void _orUint8Common(uint8_t *rhs, SUB_FUNC_PARAMS) {
    pReg->getRefA() = GET_LSB(pReg->getRefA() | (*rhs));
    GB_Flag flag;
    flag.Z = (!pReg->getRefA());
    flag.N = false;
    flag.H = false;
    flag.C = false;
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(OrA_R) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _orUint8Common(rhs, SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(OrA_MEMVAL) {
    _orUint8Common(&pInstState->memValue, SUB_FUNC_ARGS);
}

static inline void _xorUint8Common(uint8_t *rhs, SUB_FUNC_PARAMS) {
    pReg->getRefA() = GET_LSB(pReg->getRefA() ^ (*rhs));
    GB_Flag flag;
    flag.Z = (!pReg->getRefA());
    flag.N = false;
    flag.H = false;
    flag.C = false;
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(XorA_R) {
    if (uint8_t *rhs = getRegRHS(pInstState->opcode, pReg))
        _xorUint8Common(rhs, SUB_FUNC_ARGS);
}
SUB_FUNC_IMPL(XorA_MEMVAL){
    _xorUint8Common(&pInstState->memValue, SUB_FUNC_ARGS);
}

SUB_FUNC_IMPL(CCF) {
    GB_Flag flag;
    pReg->getFlag(flag);
    flag.N = false;
    flag.H = false;
    flag.C = !flag.C;
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(SCF) {
    GB_Flag flag;
    pReg->getFlag(flag);
    flag.N = false;
    flag.H = false;
    flag.C = true;
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(DAA) {
    // https://forums.nesdev.org/viewtopic.php?t=15944
    // If you want to do BCD arithmatic,
    // Call DAA after any ADD/SUB about Reg A
    static const int BCD_MAX = 0x99;
    static const int BCD_MAX_HALF = 0x9;

    // TODO: understand fast carry
    static const int FAST_CARRY = 0x60; // Do adujst without %10 and shift
    static const int FAST_CARRY_HALF = 0x06;
    GB_Flag flag;
    pReg->getFlag(flag);
    if (flag.N) { // after sub
        if (flag.C)
            pReg->getRefA() = GET_LSB(pReg->getRefA() - FAST_CARRY);
        if (flag.H)
            pReg->getRefA() = GET_LSB(pReg->getRefA() - FAST_CARRY_HALF);
    } else { // after add
        if (flag.C || pReg->getRefA() > BCD_MAX) {
            flag.C = true;
            pReg->getRefA() = GET_LSB(pReg->getRefA() + FAST_CARRY);
        }
        if (flag.H || GET_HALF_INT(pReg->getRefA()) > BCD_MAX_HALF)
            pReg->getRefA() = GET_LSB(pReg->getRefA() + FAST_CARRY_HALF);
    }
    flag.Z = !pReg->getRefA();
    flag.H = false;
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(CPL) {
    GB_Flag flag;
    pReg->getFlag(flag);
    flag.N = true;
    flag.H = true;
    pReg->setFlag(flag);
    pReg->getRefA() = ~pReg->getRefA();
}

SUB_FUNC_IMPL(LD_PC_MEM16) {
    pReg->getRefPC() = pInstState->a16Addr;
    pInstState->memMode = MEM_MODE_SLEEP;
}

SUB_FUNC_IMPL(LD_PC_HL) {
    // TODO: handle timing if execute/fetch are parallel
    pReg->getRefPC() = pReg->getRefHL();
}

static inline bool _jpConditionTrue(const uint8_t &op, const GB_Flag &flag) {
    switch((op>>3)&3) {
        case 0: return !flag.Z;
        case 1: return flag.Z;
        case 2: return !flag.C;
        case 3:
        default:
            return flag.C;
    }
}

SUB_FUNC_IMPL(CondLD_PC_MEM16) {
    GB_Flag flag;
    pReg->getFlag(flag);
    if (_jpConditionTrue(pInstState->opcode, flag)) {
        pReg->getRefPC() = pInstState->a16Addr;
        pInstState->memMode = MEM_MODE_SLEEP;
    }
}

SUB_FUNC_IMPL(JR) {
    pReg->getRefPC() += (int8_t)(pInstState->memValue);
    pInstState->memMode = MEM_MODE_SLEEP;
}

SUB_FUNC_IMPL(CondJR) {
    GB_Flag flag;
    pReg->getFlag(flag);
    if (_jpConditionTrue(pInstState->opcode, flag)) {
        pReg->getRefPC() += (int8_t)(pInstState->memValue);
        pInstState->memMode = MEM_MODE_SLEEP;
    }
}

SUB_FUNC_IMPL(SwapPC_MEM16) {
    std::swap(pReg->getRefPC(),pInstState->a16Addr);
    pInstState->memMode = MEM_MODE_SLEEP;
}

SUB_FUNC_IMPL(CondSwapPC_MEM16) {
    GB_Flag flag;
    pReg->getFlag(flag);
    if (_jpConditionTrue(pInstState->opcode, flag)) {
        std::swap(pReg->getRefPC(), pInstState->a16Addr);
        pInstState->memMode = MEM_MODE_SLEEP;
    }
}

SUB_FUNC_IMPL(Bubble) {
    pInstState->memMode = MEM_MODE_SLEEP;
}

SUB_FUNC_IMPL(CondPOP_LD_A16_LSB) {
    GB_Flag flag;
    pReg->getFlag(flag);
    if (_jpConditionTrue(pInstState->opcode, flag)) {
        pInstState->memMode = MEM_MODE_READ_ADDR_LSB;
        pInstState->memAddr = pReg->getRefSP()++;
    }
}

SUB_FUNC_IMPL(LD_PC_MEM16_RETI) {
    pReg->getRefPC() = pInstState->a16Addr;
    pInstState->memMode = MEM_MODE_SLEEP;
    pInstState->imeAction = IME_ACTION_ENABLE;
}

SUB_FUNC_IMPL(LD_MEM16_PC) {
    pInstState->a16Addr = pReg->getRefPC();
    pInstState->memMode = MEM_MODE_SLEEP;
}

static inline uint8_t __getRstAddr(const uint8_t &op) {
    return GET_LSB(((op>>3)&7)<<3);// id * 8, id = bit[3 ... 5]
}

SUB_FUNC_IMPL(RST) {
    pReg->getRefPC() = __getRstAddr(pInstState->opcode);
}

SUB_FUNC_IMPL(Halt) {
    pInstState->clockAction = CLOCK_ACTION_HALT;
}

SUB_FUNC_IMPL(StopWorkaround) {
    // TODO: stop the CPU
    // workaround for gbit only fetch first byte of op STOP
    pInstState->clockAction = CLOCK_ACTION_HALT;
}

SUB_FUNC_IMPL(Stop) {
    // TODO: stop the CPU
    if (pInstState->memValue) {
        std::cerr << "invalid stop instrcution" << std::endl;
    } else {
        pInstState->clockAction = CLOCK_ACTION_HALT;
    }
}

SUB_FUNC_IMPL(DI) {
    pInstState->imeAction = IME_ACTION_DISABLE;
}

SUB_FUNC_IMPL(EI) {
    pInstState->imeAction = IME_ACTION_ENABLE;
}

SUB_FUNC_IMPL(IncRR) {
    // TODO: emu 8 bit cpu
    ++_getCommonReg16(pInstState->opcode, pReg);
}

SUB_FUNC_IMPL(DecRR) {
    // TODO: emu 8 bit cpu
    --_getCommonReg16(pInstState->opcode, pReg);
}

SUB_FUNC_IMPL(Add_HL_RR) {
    // TODO: emu by 8 bit adder twice?
    const int lhs = pReg->getRefHL();
    const int rhs = _getCommonReg16(pInstState->opcode, pReg);
    const int  result = lhs + rhs;
    GB_Flag flag;
    pReg->getFlag(flag);
    flag.N = false;
    flag.C = result > 0xFFFF;
    flag.H = (lhs&0xFFF) + (rhs&0xFFF) > 0xFFF;
    pReg->getRefHL() = (uint16_t)(result & 0xFFFF);
    pReg->setFlag(flag);
}

static inline int _add_SP_R8(const int lhs, const int rhs, GB_Flag &flag) {
    const int result = lhs + rhs;
    flag.Z = false;
    flag.N = false;
    // TODO: is H or C correct?
    flag.H = (HALF_OP_HELPER(lhs, +, rhs) >
              HALF_UPPER_BOUND);
    flag.C = GET_LSB_INT(lhs) + GET_LSB_INT(rhs) > 0xFF;
    return result;
}

SUB_FUNC_IMPL(Add_SP_R8) {
    // TODO: get correct pipeline timing
    GB_Flag flag;
    pReg->getFlag(flag);
    const int result = _add_SP_R8(pReg->getRefSP(),
                                  (int)((int8_t)(pInstState->memValue)), flag);
    pReg->getRefSP() = (uint16_t)(result & 0xFFFF);
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(Add_HL_SP_R8) {
    // TODO: get correct pipeline timing
    GB_Flag flag;
    pReg->getFlag(flag);
    const int result = _add_SP_R8(pReg->getRefSP(),
                                  (int)((int8_t)(pInstState->memValue)), flag);
    pReg->getRefHL() = (uint16_t)(result & 0xFFFF);
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(RLCA) {
    auto &A = pReg->getRefA();
    int result = A;
    GB_Flag flag = {};
    flag.C = (1<<7)&result;
    result = (result << 1) | (flag.C ? 1 : 0);
    pReg->getRefA() = GET_LSB(result);
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(RLA) {
    auto &A = pReg->getRefA();
    int result = A;
    GB_Flag flag;
    pReg->getFlag(flag);
    result = (result << 1) | (flag.C ? 1 : 0);
    flag.C = (1<<8)&result;
    flag.Z = false;
    flag.N = false;
    flag.H = false;
    pReg->getRefA() = GET_LSB(result);
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(RRCA) {
    auto &A = pReg->getRefA();
    int result = A;
    GB_Flag flag = {};
    pReg->getFlag(flag);
    flag.C = (1)&result;
    result = (result) | (flag.C ? 1<<8 : 0);
    result >>= 1;
    flag.N = false;
    flag.Z = false;
    flag.H = false;
    pReg->getRefA() = GET_LSB(result);
    pReg->setFlag(flag);
}

SUB_FUNC_IMPL(RRA) {
    auto &A = pReg->getRefA();
    int result = A;
    GB_Flag flag;
    pReg->getFlag(flag);
    result = (result) | (flag.C ? 1<<8 : 0);
    flag.C = (1)&result;
    result >>= 1;
    flag.Z = false;
    flag.N = false;
    flag.H = false;
    pReg->getRefA() = GET_LSB(result);
    pReg->setFlag(flag);
}

}
