#include <iostream>
#include "include/lotusGB.hpp"

namespace LOTUSGB {

LotusGB::LotusGB(ICPU *pCPU, IMemoryGBIT *pMMU, IMemoryAccess *pInstructionMemory)
:pCPU(pCPU), pMMU(pMMU), pInstructionMemory(pInstructionMemory) {
}

void LotusGB::getState(LotusGBState *pState) {
    // TODO: copy all status
    Reg reg = pCPU->getReg();
    pState->pc = reg.getRefPC();
    pState->a = reg.getRefA();
    pState->f = reg.getRefF();

    pState->num_accesses = pMMU->getMemoryState(pState->mem_accesses);
}

void LotusGB::init(size_t instruction_mem_size, uint8_t *instruction_mem) {
    pMMU->setInsturctionMem(instruction_mem_size, instruction_mem);
}

void LotusGB::setState(LotusGBState *pState) {
    // TODO: copy all status
    Reg reg;
    reg.getRefA() = pState->a;
    reg.getRefF() = pState->f;
    reg.getRefPC() = pState->pc;
    pCPU->setReg(reg);

    pMMU->setMemoryState(pState->num_accesses, pState->mem_accesses);
}

int LotusGB::step() {
    // TODO: fast way to get PC
    Reg reg;
    int clockCnt = 0;
    while(reg = pCPU->getReg(), reg.getRefPC()==0) {
        pCPU->stepOneClock();
        clockCnt++;
    }
    return clockCnt;
}

}
