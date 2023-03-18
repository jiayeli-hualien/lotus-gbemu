#include "include/GBCPU.hpp"
#include "include/gb_reg.hpp"
#include "include/Iinstruction.hpp"
#include "include/instSubFunc.hpp"
#include "include/decoder.hpp"

namespace LOTUSGB {

GBCPU::GBCPU(IMemoryAccess *pMmu, Decoder *pDecoder, Decoder *pDecoderCB)
:pMmu(pMmu), pDecoder(pDecoder), pDecoderCB(pDecoderCB) {
}

IInstruction* GBCPU::fetch(uint16_t addr) {
    // TODO: handle deced failed
    const uint8_t OPCODE_CB = 0xCB;
    instStat.opcode = pMmu->read(addr);
    if (waitDecodeCB) {
        waitDecodeCB = false;
        return pDecoderCB->decode(instStat.opcode);
    }
    waitDecodeCB = (instStat.opcode == OPCODE_CB);
    return pDecoder->decode(instStat.opcode);
}

void GBCPU::fetchFirstOpcode() {
    waitDecodeCB = false;
    instStat = {};
    pInst = fetch(reg.getRefPC()++);
}

void GBCPU::reset() {
    reg = {};
    clockTimeStamp = 0;
    interruptMasterEnable = true;
    isHalt = false;
    waitDecodeCB = false;
    instStat = {};
    // load first op
    fetchFirstOpcode();
}

void GBCPU::doFetchNextOp() {
    instStat = {};
    pInst = fetch(reg.getRefPC()++);
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

bool GBCPU::stepInstruction() {
    if (!pInst) {
        std::cerr << "last decode failed" << std::endl;
        return false;
    }

    if (!pInst->stepOneMemCycle(&instStat, &reg)) {
        std::cerr << "instruction failed" << std::endl;
        return false;
    }
    return true;
}

RetStepOneCycle GBCPU::stepOneCycle() {
    RetStepOneCycle ret;
    instStat.memMode = MEM_MODE_FETCH;
    instStat.imeAction = IME_ACTION_NONE;
    instStat.clockAction = CLOCK_ACTION_NONE;

    if (!stepInstruction())
        return ret;

    switch (instStat.imeAction) { // TODO: let instruction fully control IME?
        case IME_ACTION_ENABLE:
            setIME(true); break;
        case IME_ACTION_DISABLE:
            setIME(false); break;
        case IME_ACTION_NONE:
        default:
            break;
    }

    if (instStat.clockAction == CLOCK_ACTION_HALT)
        setHALT(true);

    ret.fetchedNewInst = (!waitDecodeCB)
                         && (instStat.memMode == MEM_MODE_FETCH);

    switch (instStat.memMode) {
        case MEM_MODE_FETCH: doFetchNextOp(); break;
        case MEM_MODE_READ: doMemRead(instStat); break;
        case MEM_MODE_WRITE: doMemWrite(instStat); break;
        case MEM_MODE_READ_ADDR_LSB: doMemReadAddrLSB(instStat); break;
        case MEM_MODE_READ_ADDR_MSB: doMemReadAddrMSB(instStat); break;
        case MEM_MODE_SLEEP:
        default:
            break;
    }
    clockTimeStamp+=4;
    ret.success = true;
    return ret;
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