#include "include/GBCPU.hpp"
#include "include/gb_reg.hpp"

// TODO: instruction factory, instruction from csv
#include "include/instSubFunc.hpp"

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
    InstState instStat;
    // TODO: fetch
    instStat.inst[instStat.fetchedLen++] = pMmu->read(reg.getRefPC()++);
    if (instStat.inst[0] == 0x00) {
        subFuncNOP nop;
        nop(&instStat, &reg);
    } else if ((instStat.inst[0]&0xc0) == 0x40) {
        subFuncLDRR ldrr;
        ldrr(&instStat, &reg);
    }
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