#include <iostream>
#include <cstddef>
#include <cstdint>
#include "include/instSubFunc.hpp"
#include "include/instruction.hpp"
#include "include/gb_reg.hpp"

namespace LOTUSGB {

static uint8_t* _ldrrGetReg(int number, Reg *reg) {
    switch (number) {
        case 0: return &reg->getRefB();
        case 1: return &reg->getRefC();
        case 2: return &reg->getRefD();
        case 3: return &reg->getRefE();
        case 4: return &reg->getRefH();
        case 5: return &reg->getRefL();
        // 6: HL is handled in 16bit LD instruction
        case 7: return &reg->getRefA();
        default:
            return nullptr;
    }
}

static uint8_t* ldrrGetRegLHS(uint8_t op, Reg *reg) {
    constexpr uint8_t MASK = 0x3F;
    const int num = ((op&MASK)>>3U);
    return _ldrrGetReg(num, reg);
}

static uint8_t* ldrrGetRegRHS(uint8_t op, Reg *reg) {
    constexpr uint8_t MASK = 0x07;
    const int num = (op & MASK);
    return _ldrrGetReg(num, reg);
}


SUB_FUNC_IMPL(subFuncNOP){
    return;
}

SUB_FUNC_IMPL(subFuncLDRR) {
    // TODO: error handling
    const uint8_t op = instState->inst[0];
    uint8_t *lhs = ldrrGetRegLHS(op, reg);
    uint8_t *rhs = ldrrGetRegRHS(op, reg);
    if (!lhs || !rhs) {
        std::cout << "invalid OP code, reg not support" << (int)op << std::endl;
        return;
    }
    *lhs = *rhs;
}

}