#ifndef LOTUSGB_GBCPU_HPP
#define LOTUSGB_GBCPU_HPP

#include "ICPU.hpp"
#include "IMemoryAccess.hpp"
#include "gb_reg.hpp"
#include "decoder.hpp"

namespace LOTUSGB {

// TODO: refactor the naming: GBCPU->GbCpu, find a tool
class GBCPU : public ICPU {
public:
    GBCPU(IMemoryAccess *pMmu, Decoder *pDecoder);
    void reset();
    bool stepOneCycle();
    int stepOneInstruction();

    void fetchFirstOpcode();
    Reg getReg();
    void setReg(const Reg &reg);
    bool getIME();
    void setIME(bool ime);
    bool getHALT();
    void setHALT(bool halt);
private:
    void fetch(uint16_t addr);
    void doFetchNextOp();
    void doMemRead(InstState &instStat);
    void doMemWrite(InstState &instStat);

    IMemoryAccess *pMmu;
    Reg reg;
    u_int64_t clockTimeStamp;
    bool interruptMasterEnable = true;
    bool isHalt = false;
    static constexpr size_t INST_BUFFER_SIZE_POW2 = 1;
    static constexpr size_t INST_BUFFER_SIZE = 1<<INST_BUFFER_SIZE_POW2;
    InstState instStatBuf[INST_BUFFER_SIZE] = {};
    IInstruction *pInstBuf[INST_BUFFER_SIZE] = {};
    int curInst = 0;
    Decoder *pDecoder = nullptr;
};

}

#endif // LOTUSGB_CPU_HPP
