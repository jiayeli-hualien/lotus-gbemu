#ifndef LOTUSGB_GBITCPU_HPP
#define LOTUSGB_GBITCPU_HPP

#include<iostream>
#include <iostream>
#include "ICPU.hpp"
#include "IGBITCPU.hpp"
#include "gb_reg.hpp"

namespace LOTUSGB {

class GBITCPU : public IGBITCPU {
public:
    virtual int stepOneInstruction();
    GBITCPU(ICPU *pCPU);
private:
    ICPU *pCPU = NULL;
};

}

#endif // LOTUSGB_GBITCPU_HPP
