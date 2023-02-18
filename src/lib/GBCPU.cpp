#include "include/GBCPU.hpp"
#include "include/gb_reg.hpp"
#include "include/Iinstruction.hpp"
#include "include/instSubFunc.hpp"
#include "include/decoder.hpp"

namespace LOTUSGB {

GBCPU::GBCPU(IMemoryAccess *pMmu, Decoder *pDecoder):pMmu(pMmu), pDecoder(pDecoder) {
}

void GBCPU::fetch(uint16_t addr) {
    InstState &instStat = instStatBuf[curInst];
    instStat.inst[instStat.fetchedLen++] = pMmu->read(addr);
    if (instStat.fetchedLen == 1) {
        pInstBuf[curInst] = pDecoder->decode(instStat.inst[0]);
    }
}

void GBCPU::fetchFirstOpcode() {
    fetch(reg.getRefPC()++);
}

void GBCPU::reset() {
    reg = {};
    clockTimeStamp = 0;
    interruptMasterEnable = true;
    isHalt = false;
    for (auto &stat: instStatBuf)
        stat = {};
    curInst = 0;
    // load first op
    fetchFirstOpcode();
}

void GBCPU::doFetchNextOp() {
    constexpr int MASK = INST_BUFFER_SIZE-1;
    curInst = (curInst+1)&MASK;
    instStatBuf[curInst] = {};
    pInstBuf[curInst] = nullptr;
    fetch(reg.getRefPC()++);
}

void GBCPU::doMemRead(InstState &instStat) {
    instStat.memValue = pMmu->read(instStat.memAddr);
}
void GBCPU::doMemWrite(InstState &instStat) {
    pMmu->write(instStat.memAddr,  instStat.memValue);
}

bool GBCPU::stepOneCycle() {
    InstState &instStat = instStatBuf[curInst];
    IInstruction * const pInst = pInstBuf[curInst];
    if (!pInst) {
        std::cerr << "last decode failed" << std::endl;
        return false;
    }
    instStat.memMode = MEM_MODE_NONE;
    if (!pInst->stepOneMemCycle(&instStat, &reg)) {
        std::cerr << "instruction failed" << std::endl;
    }
    switch (instStat.memMode) {
        case MEM_MODE_NONE: doFetchNextOp(); break;
        case MEM_MODE_READ: doMemRead(instStat); break;
        case MEM_MODE_WRITE: doMemWrite(instStat); break;
        default:
            break;
    }
    clockTimeStamp+=4;
    return true;
}

int GBCPU::stepOneInstruction() {
    int lastInst = curInst;
    int cntMemCycle = 0;
    while (lastInst == curInst && stepOneCycle()) {
        cntMemCycle++;
    }
    return cntMemCycle;
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