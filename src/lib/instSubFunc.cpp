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
    return _getReg(num, pReg);
}

static uint8_t* getRegRHS(const uint8_t &op, Reg *pReg) {
    constexpr uint8_t MASK = 0x07;
    const int num = (op & MASK);
    return _getReg(num, pReg);
}


SUB_FUNC_IMPL(subFuncNOP){
    return;
}

SUB_FUNC_IMPL(subFuncLDRR) {
    // TODO: error handling
    const uint8_t &op = pInstState->inst[0];
    uint8_t *lhs = getRegLHS(op, pReg);
    uint8_t *rhs = getRegRHS(op, pReg);
    if (!lhs || !rhs) {
        std::cerr << "invalid OP code, reg not support" << (int)op << std::endl;
        return;
    }
    *lhs = *rhs;
}

SUB_FUNC_IMPL(subFuncMemRead) {
    // notify CPU to read immediate value
    pInstState->memMode = MEM_MODE_READ;
}

SUB_FUNC_IMPL(subFuncLDR_IMMD) {
    const uint8_t op = pInstState->inst[0];
    uint8_t *lhs = getRegLHS(op, pReg);
    if (!lhs) {
        std::cerr << "invalid OP code, reg not support" << (int)op << std::endl;
        return;
    }
    *lhs = pInstState->memValue;
}

}