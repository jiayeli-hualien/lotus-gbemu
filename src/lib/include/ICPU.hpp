#ifndef LOTUSGB_ICPU_HPP
#define LOTUSGB_ICPU_HPP

#include<iostream>
#include <iostream>
#include "gb_reg.hpp"

namespace LOTUSGB {

class ICPU {
public:
    virtual void reset() = 0;
    // step one CPU clock
    virtual void stepOneClock() = 0;

    // tool functions for GBIT
    virtual Reg getReg() = 0;
    virtual void setReg(const Reg &reg) = 0;
};

}

#endif // LOTUSGB_ICPU_HPP
