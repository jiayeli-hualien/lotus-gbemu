#ifndef LOTUSGB_INSTRUCTION_HPP
#define LOTUSGB_INSTRUCTION_HPP
#include <string>
#include <vector>
#include <functional>
#include "Iinstruction.hpp"
#include "IMemoryAccess.hpp"
#include "gb_reg.hpp"

namespace LOTUSGB {

// TODO: think a beter way to handle it
using SUB_FUNC_OP_RET = void;
#define SUB_FUMC_ARGS InstState *instState, Reg *reg, IMemoryAccess *pMmu
#define SUB_FUNC_OP operator () (SUB_FUMC_ARGS)

struct subFuncMemRead {
    SUB_FUNC_OP_RET SUB_FUNC_OP {
        instState->memRW = true;
    }
};

struct subFuncMemWrite {
    SUB_FUNC_OP_RET SUB_FUNC_OP {
        instState->memRW = true;
    }
};

class Instruction : public IInstruction {
public:
    void stepOneMemCycle(SUB_FUMC_ARGS);
private:
    std::string name;
    int memCycles;
    using FUNC_TYPE =
        std::function<SUB_FUNC_OP_RET(SUB_FUMC_ARGS)>;
    std::vector<FUNC_TYPE> funcPerMemCycle;
};

}

#endif // LOTUSGB_IINSTRUCTION_HPP