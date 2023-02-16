#include "include/GBCPU.hpp"
#include "include/gb_reg.hpp"

namespace LOTUSGB {

GBCPU::GBCPU(IMemoryAccess *pMmu):pMmu(pMmu) {

}

void GBCPU::reset() {
    // TODO: PC = 0, or reset intterupt
    reg.getRefPC() = 0;
    clockTimeStamp = 0;
}

void GBCPU::stepOneCycle() {
    // TODO: opcode framework
    // Only handle NOP now
    reg.getRefPC()++;
    clockTimeStamp+=4;
}

Reg GBCPU::getReg() {
    return reg;
}

void GBCPU::setReg(const Reg &reg) {
    this->reg = reg;
}

bool GBCPU::getIME() {
    return this->interruptMasterEnable;
}

void GBCPU::setIME(bool ime) {
    this->interruptMasterEnable = ime;
}

bool GBCPU::getHALT() {
    return this->isHalt;
}

void GBCPU::setHALT(bool halt) {
    this->isHalt = halt;
}

}