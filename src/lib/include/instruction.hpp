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
#define SUB_FUNC_PARAMS InstState *pInstState, Reg *pReg
#define SUB_FUNC_ARGS pInstState, pReg
#define SUB_FUNC_OP operator () (SUB_FUNC_PARAMS)

class Instruction : public IInstruction {
public:
    using FUNC_TYPE =
        std::function<SUB_FUNC_OP_RET(SUB_FUNC_PARAMS)>;
    Instruction(std::string name, int memCycle,
                std::vector<FUNC_TYPE> &&funcList);
    bool stepOneMemCycle(SUB_FUNC_PARAMS);
private:
    std::string name;
    int memCycles;
    std::vector<FUNC_TYPE> funcPerMemCycle;
};

}

#endif // LOTUSGB_IINSTRUCTION_HPP