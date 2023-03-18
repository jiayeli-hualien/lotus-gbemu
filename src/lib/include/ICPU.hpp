#ifndef LOTUSGB_ICPU_HPP
#define LOTUSGB_ICPU_HPP

#include<iostream>
#include <iostream>
#include "gb_reg.hpp"

namespace LOTUSGB {

struct RetStepOneCycle {
    bool success = false;
    bool fetchedNewInst = false;
};

class ICPU {
public:
    virtual void reset() = 0;
    virtual void fetchFirstOpcode() = 0;

    // step one memory cycle
    virtual RetStepOneCycle stepOneCycle() = 0; // 1Hz mem cycle
    // TODO: func stepOneCPUClock

    // tool functions for GBIT
    // TODO: remove them from API, or provide Memento Pattern
    virtual Reg getReg() = 0;
    virtual void setReg(const Reg &reg) = 0;
    virtual bool getIME() = 0;
    virtual void setIME(bool ime) = 0;
    virtual bool getHALT() = 0;
    virtual void setHALT(bool halt) = 0;
};

}

#endif // LOTUSGB_ICPU_HPP
