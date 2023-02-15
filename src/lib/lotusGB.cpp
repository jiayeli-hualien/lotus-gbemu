#include <iostream>
#include <cstring>
#include "include/lotusGB.hpp"

namespace LOTUSGB {

LotusGB::LotusGB(ICPU *pCPU, IMemoryGBIT *pMMU, IMemoryAccess *pInstructionMemory)
:pCPU(pCPU), pMMU(pMMU), pInstructionMemory(pInstructionMemory) {
}

void LotusGB::getState(LotusGBState *pState) {
    // TODO: copy all status
    memset(pState, 0, sizeof(LotusGBState));
    Reg reg = pCPU->getReg();
    pState->pc = reg.getRefPC();
    pState->sp = reg.getRefSP();
    pState->a = reg.getRefA();
    pState->f = reg.getRefF();
    pState->b = reg.getRefB();
    pState->c = reg.getRefC();
    pState->d = reg.getRefD();
    pState->e = reg.getRefE();
    pState->h = reg.getRefH();
    pState->l = reg.getRefL();
    pState->is_halt = pCPU->getHALT();
    pState->interrupts_master_enable = pCPU->getIME();
    pState->num_accesses = pMMU->getMemoryState(pState->mem_accesses);
}

void LotusGB::init(size_t instruction_mem_size, uint8_t *instruction_mem) {
    pMMU->setInsturctionMem(instruction_mem_size, instruction_mem);
}

void LotusGB::setState(LotusGBState *pState) {
    // TODO: copy all status
    Reg reg;
    reg.getRefPC() = pState->pc;
    reg.getRefSP() = pState->sp;
    reg.getRefA() = pState->a;
    reg.getRefF() = pState->f;
    reg.getRefB() = pState->b;
    reg.getRefC() = pState->c;
    reg.getRefD() = pState->d;
    reg.getRefE() = pState->e;
    reg.getRefH() = pState->h;
    reg.getRefL() = pState->l;
    pCPU->setReg(reg);
    pCPU->setHALT(pState->is_halt);
    pCPU->setIME(pState->interrupts_master_enable);
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
