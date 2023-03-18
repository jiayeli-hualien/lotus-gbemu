#include "include/GBITCPU.hpp"
#include "include/ICPU.hpp"

namespace LOTUSGB {

GBITCPU::GBITCPU(ICPU *pCPU): pCPU(pCPU) {}

int GBITCPU::stepOneInstruction() {
    int cntMemCycle = 0;
    RetStepOneCycle res;
    while (res = pCPU -> stepOneCycle(),
            res.success && !res.fetchedNewInst)
        cntMemCycle++;
    return cntMemCycle;
}

}
