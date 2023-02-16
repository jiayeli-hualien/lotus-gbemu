#ifndef LOTUSGB_IINSTRUCTION_HPP
#define LOTUSGB_IINSTRUCTION_HPP
#include <cstdint>
#include <cstddef>
#include "IMemoryAccess.hpp"
#include "gb_reg.hpp"

namespace LOTUSGB {

static constexpr size_t MAX_INST_LEN = 4; // in bytes, acutaly 3 but align 4
struct InstState {
    uint8_t inst[MAX_INST_LEN] = {}; // fetching instruction
    size_t fetchedLen = 0;

    int memCycleCnt = 0;
    bool memRW = false; // can't fetch next byte int this clock
    uint8_t memValue[2] = {}; // value read from memory or will to write to memory
    int memAccessCnt = 0; // read/write cnt

    // TODO: system clock / interrupt handler
    bool setHalt = false;
    bool nextHaltValue = false;
    bool setIME = false;
    bool nextIMEValue = false;
};

class IInstruction {
// Stateless interface/object, state from InstructState
// Child class can hold read-only data/function pointer
public:
    virtual void stepOneMemCycle(InstState *instState, Reg *reg,
                                 IMemoryAccess *pMmu) = 0;
};

}

#endif // LOTUSGB_IINSTRUCTION_HPP