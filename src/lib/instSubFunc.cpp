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
    switch(op) {
        case 0x02:
        case 0x12:
            return &(pReg->getRefA());
        default:
            constexpr uint8_t MASK = 0x07;
            const int num = (op & MASK);
            auto ret = _getReg(num, pReg);
            return ret;
    }
}


subFunMapType getSubFuncMap() {
    subFunMapType map;
    // TODO: codegen?
    map["subFuncNOP"] = subFuncNOP();
    map["subFuncLDRR"] = subFuncLDRR();
    map["subFuncMemReadPC"] = subFuncMemReadPC();
    map["subFuncMemReadIndirect"] = subFuncMemReadIndirect();
    map["subFuncMemWriteIndirect"] = subFuncMemWriteIndirect();
    map["subFuncLD_R_MEMVAL"] = subFuncLD_R_MEMVAL();
    map["subFuncLD_A_MEMVAL"] = subFuncLD_A_MEMVAL();
    map["subFuncReadA16LSB"] = subFuncReadA16LSB();
    map["subFuncReadA16MSB"] = subFuncReadA16MSB();

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

static uint16_t& _getIndreictReg16(InstState *pInstState, Reg *pReg) {
    switch(pInstState->opcode) {
        case 0x02:
        case 0x0A:
            return pReg->getRefBC();
        case 0x12:
        case 0x1A:
            return pReg->getRefDE();
        case 0xFA:
            return pInstState->a16Addr;
        default:
            return pReg->getRefHL();
    }
}

SUB_FUNC_IMPL(subFuncMemReadIndirect) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = _getIndreictReg16(pInstState, pReg);
}

SUB_FUNC_IMPL(subFuncReadA16LSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_LSB;
    pInstState->memAddr = pReg->getRefPC()++;
}

SUB_FUNC_IMPL(subFuncReadA16MSB) {
    pInstState->memMode = MEM_MODE_READ_ADDR_MSB;
    pInstState->memAddr = pReg->getRefPC()++;
}

SUB_FUNC_IMPL(subFuncMemWriteIndirect) {
    pInstState->memMode = MEM_MODE_WRITE;
    pInstState->memAddr = _getIndreictReg16(pInstState, pReg);
    uint8_t *rhs = getRegRHS(pInstState->opcode, pReg);
    if (rhs)
        pInstState->memValue = *rhs;
}

SUB_FUNC_IMPL(subFuncLD_R_MEMVAL) {
    uint8_t *lhs = getRegLHS(pInstState->opcode, pReg);
    if (lhs)
        *lhs = pInstState->memValue;
}

SUB_FUNC_IMPL(subFuncLD_A_MEMVAL) {
    pReg->getRefA() = pInstState->memValue;
}

}
