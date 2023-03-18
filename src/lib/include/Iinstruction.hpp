#ifndef LOTUSGB_IINSTRUCTION_HPP
#define LOTUSGB_IINSTRUCTION_HPP
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <functional>
#include "instSubFuncMacro.hpp"
#include "gb_reg.hpp"

namespace LOTUSGB {

static const uint16_t HIGH_MEM_ADDR = 0xFF00;
enum MEM_MODE {
    MEM_MODE_FETCH,
    MEM_MODE_READ,
    MEM_MODE_WRITE,
    MEM_MODE_READ_ADDR_LSB, // TODO: let instruction access memory?
    MEM_MODE_READ_ADDR_MSB,
    MEM_MODE_SLEEP,
};

enum IME_ACTION {
    IME_ACTION_NONE = 0,
    IME_ACTION_ENABLE,
    IME_ACTION_DISABLE,
};

enum CLOCK_ACTION {
    CLOCK_ACTION_NONE = 0,
    CLOCK_ACTION_HALT,
};

static constexpr size_t MAX_INST_LEN = 4; // in bytes, acutaly 3 but align 4
struct InstState {
    uint8_t opcode = 0;

    int memCycleCnt = 0; // current memCycleCnt of this instruction
    MEM_MODE memMode = MEM_MODE_FETCH;
    uint8_t memValue = 0; // value read from memory or will to write to memory
    uint16_t memAddr = 0;
    // TODO: find better place for a16addr, rename to a16
    uint16_t a16Addr = 0;

    // TODO: system clock / interrupt handler
    bool setHalt = false;
    bool nextHaltValue = false;
    bool setIME = false;
    bool nextIMEValue = false;

    IME_ACTION imeAction = IME_ACTION_NONE;
    CLOCK_ACTION clockAction = CLOCK_ACTION_NONE;
};

InstState* createInitState();
void destoryInitState(InstState* pInstState);
void clearInitState(InstState* pInstState);

using SUB_FUNC_OP_RET = void;

#define INST_USE_FUNC_OBJ 1 // Use func obj or func ptr for FUNC_TYPE

class IInstruction {
// Stateless interface/object, state from InstructState
// Child class can hold read-only data/function pointer
public:
#if INST_USE_FUNC_OBJ
    using FUNC_TYPE =
        std::function<SUB_FUNC_OP_RET(SUB_FUNC_PARAMS)>;
#else // INST_USE_FUNC_OBJ
    using FUNC_TYPE = SUB_FUNC_OP_RET(*)(SUB_FUNC_PARAMS);
#endif // INST_USE_FUNC_OBJ
    virtual bool stepOneMemCycle(SUB_FUNC_PARAMS) = 0;
};

using subFunMapType = std::unordered_map<std::string, IInstruction::FUNC_TYPE>;

}

#endif // LOTUSGB_IINSTRUCTION_HPP