/*
A simple C++ class to provide registers set and get
*/
#ifndef LOTUSGB_REG_H
#define LOTUSGB_REG_H
#include <cstdint>
#include <cmath>

namespace LOTUSGB {

// little endian host
enum REG_ID {
    REG_ID_A = 1,
    REG_ID_F = 0,
    REG_ID_B = 3,
    REG_ID_C = 2,
    REG_ID_D = 5,
    REG_ID_E = 4,
    REG_ID_H = 7,
    REG_ID_L = 6,
};
// TODO: big endian host


#define CAST_REF_TO_16BIT(x) (*((uint16_t*)(&x)))
#define REG_GET_BIT_N(x, n) (((uint32_t)(x)>>((uint32_t)(n)))&1U)
#define REG_U32_L_SHIFT(x, n) (((uint32_t)(x))<<((uint32_t)(n)))

struct GB_Flag {
  bool Z = false; // result is zero
  bool N = false;
  bool H = false; // half carry
  bool C = false; // carry
};

class Reg {
    static const int REG_8BIT_NUM = 8;
    uint8_t reg8bit[REG_8BIT_NUM] = {};

    uint16_t sp = 0;
    uint16_t pc = 0;
    uint16_t a16Addr = 0;
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

    inline uint16_t& getRefBC() {
        // TODO: Can it handle different endian?
        constexpr int idx_BC = std::min(REG_ID_B, REG_ID_C);
        return CAST_REF_TO_16BIT(reg8bit[idx_BC]);
    }
    inline uint16_t& getRefDE() {
        constexpr int idx_DE = std::min(REG_ID_D, REG_ID_E);
        return CAST_REF_TO_16BIT(reg8bit[idx_DE]);
    }
    inline uint16_t& getRefHL() {
        constexpr int idx_HL = std::min(REG_ID_H, REG_ID_L);
        return CAST_REF_TO_16BIT(reg8bit[idx_HL]);
    }
    inline uint16_t& getRefAF() {
        constexpr int idx_AF = std::min(REG_ID_A, REG_ID_F);
        return CAST_REF_TO_16BIT(reg8bit[idx_AF]);
    }
    inline uint16_t& getRefPC() { return pc; };
    inline uint16_t& getRefSP() { return sp; };
    inline GB_Flag getFlag() {
        GB_Flag ret;
        getFlag(ret);
        return ret;
    }
    inline void getFlag(GB_Flag &flag) {
        const uint8_t &f = getRefF();
        flag.Z = (REG_GET_BIT_N(f, 7U));
        flag.N = (REG_GET_BIT_N(f, 6U));
        flag.H = (REG_GET_BIT_N(f, 5U));
        flag.C = (REG_GET_BIT_N(f, 4U));
    };
    inline void setFlag(GB_Flag &flag) {
        uint8_t &f = getRefF();
        f = (uint8_t)(
            REG_U32_L_SHIFT(flag.Z, 7U)|
            REG_U32_L_SHIFT(flag.N, 6U)|
            REG_U32_L_SHIFT(flag.H, 5U)|
            REG_U32_L_SHIFT(flag.C, 4U));
    };
};

}

#endif // LOTUSGB_GBREG_C_H
