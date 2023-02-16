#ifndef LOTUSGB_GBCPU_HPP
#define LOTUSGB_GBCPU_HPP

#include "ICPU.hpp"
#include "IMemoryAccess.hpp"
#include "gb_reg.hpp"

namespace LOTUSGB {

// TODO: refactor the naming: GBCPU->GbCpu, find a tool
class GBCPU : public ICPU {
public:
    GBCPU(IMemoryAccess *pMmu);
    void reset();
    void stepOneCycle();
    Reg getReg();
    void setReg(const Reg &reg);
    bool getIME();
    void setIME(bool ime);
    bool getHALT();
    void setHALT(bool halt);
private:
    IMemoryAccess *pMmu;
    Reg reg;
    u_int64_t clockTimeStamp;
    bool interruptMasterEnable;
    bool isHalt;
};

}

#endif // LOTUSGB_CPU_HPP
