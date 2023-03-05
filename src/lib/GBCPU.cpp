#include "include/GBCPU.hpp"
#include "include/gb_reg.hpp"
#include "include/Iinstruction.hpp"
#include "include/instSubFunc.hpp"
#include "include/decoder.hpp"

namespace LOTUSGB {

GBCPU::GBCPU(IMemoryAccess *pMmu, Decoder *pDecoder):pMmu(pMmu), pDecoder(pDecoder) {
}

void GBCPU::fetch(uint16_t addr) {
    pInst = pDecoder->decode(instStat.opcode = pMmu->read(addr));
}

void GBCPU::fetchFirstOpcode() {
    fetch(reg.getRefPC()++);
}

void GBCPU::reset() {
    reg = {};
    clockTimeStamp = 0;
    interruptMasterEnable = true;
    isHalt = false;
    instStat = {};
    curInst = 0;
    // load first op
    fetchFirstOpcode();
}

void GBCPU::doFetchNextOp() {
    constexpr int MASK = INST_BUFFER_SIZE-1;
    curInst = (curInst+1)&MASK;
    instStat = {};
    pInst = nullptr;
    fetch(reg.getRefPC()++);
}

void GBCPU::doMemRead(InstState &instStat) {
    instStat.memValue = pMmu->read(instStat.memAddr);
}
void GBCPU::doMemReadAddrLSB(InstState &instStat) {
    const uint8_t tmp = pMmu->read(instStat.memAddr);
    instStat.a16Addr = HIGH_MEM_ADDR | tmp;
}
void GBCPU::doMemReadAddrMSB(InstState &instStat) {
    const uint8_t tmp = pMmu->read(instStat.memAddr);
    instStat.a16Addr = (tmp << 8U) | (instStat.a16Addr & 0xFF);
}
void GBCPU::doMemWrite(InstState &instStat) {
    pMmu->write(instStat.memAddr,  instStat.memValue);
}

bool GBCPU::stepOneCycle() {
    if (!pInst) {
        std::cerr << "last decode failed" << std::endl;
        return false;
    }
    instStat.memMode = MEM_MODE_NONE;
    instStat.imeAction = IME_ACTION_NONE;
    instStat.clockAction = CLOCK_ACTION_NONE;
    if (!pInst->stepOneMemCycle(&instStat, &reg)) {
        std::cerr << "instruction failed" << std::endl;
    }
    switch (instStat.imeAction) { // TODO: let instruction fully control IME?
        case IME_ACTION_ENALBE:
            setIME(true); break;
        case IME_ACTION_DISABLE:
            setIME(false); break;
        case IME_ACTION_NONE:
        default:
            break;
    }
    if (instStat.clockAction == CLOCK_ACTION_HALT)
        setHALT(true);
    switch (instStat.memMode) {
        case MEM_MODE_NONE: doFetchNextOp(); break;
        case MEM_MODE_READ: doMemRead(instStat); break;
        case MEM_MODE_WRITE: doMemWrite(instStat); break;
        case MEM_MODE_READ_ADDR_LSB: doMemReadAddrLSB(instStat); break;
        case MEM_MODE_READ_ADDR_MSB: doMemReadAddrMSB(instStat); break;
        case MEM_MODE_SLEEP:
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