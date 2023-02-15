/*
A simple C++ class to provide registers set and get
*/
#ifndef LOTUSGB_REG_H
#define LOTUSGB_REG_H
#include <cstdint>

namespace LOTUSGB {

// little endian host
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
// TODO: big endian host


#define CAST_REF_TO_16BIT(x) (*((uint16_t*)(&x)))

class Reg {
    static const int REG_8BIT_NUM = 8;
    uint8_t reg8bit[REG_8BIT_NUM] = {};

    uint16_t sp = 0;
    uint16_t pc = 0;
public:
    // TODO: more regs
    // TODO: move function to a .cpp file
    inline uint8_t& getRefA() { return reg8bit[REG_ID_A]; };
    inline uint8_t& getRefF() { return reg8bit[REG_ID_F]; };
    inline uint8_t& getRefB() { return reg8bit[REG_ID_B]; };
    inline uint8_t& getRefC() { return reg8bit[REG_ID_C]; };
    inline uint8_t& getRefD() { return reg8bit[REG_ID_D]; };
    inline uint8_t& getRefE() { return reg8bit[REG_ID_E]; };
    inline uint8_t& getRefH() { return reg8bit[REG_ID_H]; };
    inline uint8_t& getRefL() { return reg8bit[REG_ID_L]; };

    inline uint16_t& getRefBC() { return CAST_REF_TO_16BIT(reg8bit[REG_ID_B]);}
    inline uint16_t& getRefDE() { return CAST_REF_TO_16BIT(reg8bit[REG_ID_D]);}
    inline uint16_t& getRefHL() { return CAST_REF_TO_16BIT(reg8bit[REG_ID_H]);}
    inline uint16_t& getRefPC() { return pc; };
    inline uint16_t& getRefSP() { return sp; };
};

}

#endif // LOTUSGB_GBREG_C_H
