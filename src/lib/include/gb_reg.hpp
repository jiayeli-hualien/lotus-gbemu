/*
A simple C++ class to provide registers set and get
*/
#ifndef LOTUSGB_REG_H
#define LOTUSGB_REG_H
#include <cstdint>

namespace LOTUSGB {

enum REG_ID {
    REG_ID_A = 0,
    REG_ID_F,
    REG_ID_B,
    REG_ID_C,
    REG_ID_D,
    REG_ID_E,
    REG_ID_H,
    REG_ID_L,
};

class Reg {
    static const int REG_8BIT_NUM = 8;
    uint8_t reg8bit[REG_8BIT_NUM];

    uint16_t sp;
    uint16_t pc;
public:
    // TODO: more regs
    uint8_t& getRefA() { return reg8bit[REG_ID_A]; };
};

}

#endif // LOTUSGB_GBREG_C_H
