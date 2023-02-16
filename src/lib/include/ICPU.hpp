#ifndef LOTUSGB_ICPU_HPP
#define LOTUSGB_ICPU_HPP

#include<iostream>
#include <iostream>
#include "gb_reg.hpp"

namespace LOTUSGB {

class ICPU {
public:
    virtual void reset() = 0;

    // step one memory cycle until got cpu pipeline documents
    // TODO: func stepOneCPUClock
    virtual void stepOneCycle() = 0; // 1Hz mem cycle

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
