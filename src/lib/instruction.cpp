#include <iostream>
#include <string>
#include <cstring>
#include "include/instruction.hpp"

namespace LOTUSGB {

InstState* createInitState() {
    return new InstState;
}

void destoryInitState(InstState* pInstState) {
    delete pInstState;
}

void clearInitState(InstState* pInstState) {
    *pInstState = {};
}

Instruction::Instruction(std::string name, int memCycle,
std::vector<FUNC_TYPE> &&funcList):name(name), memCycles(memCycle), funcPerMemCycle(funcList)
{
    if (memCycles != (int)funcList.size())
        std::cerr << "invalid instruction size" << std::endl;
}

bool Instruction::stepOneMemCycle(SUB_FUNC_PARAMS) {
    funcPerMemCycle[pInstState->memCycleCnt](SUB_FUNC_ARGS);
    return ++pInstState->memCycleCnt < memCycles;
}

}