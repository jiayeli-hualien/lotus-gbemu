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

}
