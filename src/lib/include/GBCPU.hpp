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
    void stepOneClock();
    Reg getReg();
    void setReg(const Reg &reg);
private:
    IMemoryAccess *pMmu;
    Reg reg;
    u_int64_t clockTimeStamp;
};

}

#endif // LOTUSGB_CPU_HPP
