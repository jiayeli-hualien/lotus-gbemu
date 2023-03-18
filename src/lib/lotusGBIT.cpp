#include <iostream>
#include <cstring>
#include "include/lotusGBIT.hpp"
#include "include/GBITCPU.hpp"

namespace LOTUSGB {

LotusGBIT::LotusGBIT(ICPU *pCPU, IMemoryGBIT *pMMU)
:pCPU(pCPU), pMMU(pMMU) {
    pGBITCPU = new GBITCPU(pCPU);
}

LotusGBIT::~LotusGBIT() {
    delete pGBITCPU;
}

void LotusGBIT::getState(LotusGBState *pState) {
    // TODO: copy all status
    memset(pState, 0, sizeof(LotusGBState));
    Reg reg = pCPU->getReg();
    // gbit reference CPU don't have parallel fetch/excute overlap
    pState->pc = reg.getRefPC() - 1;
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

void LotusGBIT::init(size_t instruction_mem_size, uint8_t *instruction_mem) {
    pMMU->setInsturctionMem(instruction_mem_size, instruction_mem);
}

void LotusGBIT::setState(LotusGBState *pState) {
    // TODO: copy all status
    Reg reg;
    pCPU->reset();
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

    // workaround for pCPU->reset / setReg order
    pCPU->fetchFirstOpcode();
}

int LotusGBIT::step() {
   return pGBITCPU->stepOneInstruction()*4;
}

}
