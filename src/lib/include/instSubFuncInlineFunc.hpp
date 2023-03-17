#include <iostream>
#include <cstddef>
#include <cstdint>
#include "gb_reg.hpp"

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

static uint8_t* getRegRHS(const uint8_t &op, Reg *pReg) {
    constexpr uint8_t MASK = 0x07;
    const int num = (op & MASK);
    auto ret = _getReg(num, pReg);
    return ret;
}

}