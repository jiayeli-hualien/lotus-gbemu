#ifndef LOTUSGB_INSTRUCTION_HPP
#define LOTUSGB_INSTRUCTION_HPP
#include "Iinstruction.hpp"
#include "IMemoryAccess.hpp"
#include "gb_reg.hpp"

namespace LOTUSGB {

// TODO: think a beter way to handle it

class Instruction : public IInstruction {
public:
    Instruction();
    Instruction(std::string name, int memCycle,
                std::vector<IInstruction::FUNC_TYPE> &&funcList);
    bool stepOneMemCycle(SUB_FUNC_PARAMS);
    std::string getName();
private:
    std::string name = std::string();
    int memCycles = 0;
    std::vector<IInstruction::FUNC_TYPE> funcPerMemCycle = {};
};

}

#endif // LOTUSGB_IINSTRUCTION_HPP