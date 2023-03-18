#ifndef LOTUSGB_IGBITCPU_HPP
#define LOTUSGB_IGBITCPU_HPP

#include<iostream>
#include <iostream>
#include "gb_reg.hpp"

namespace LOTUSGB {

class IGBITCPU {
public:
    virtual ~IGBITCPU() {} ;
    virtual int stepOneInstruction() = 0;
};

}

#endif // LOTUSGB_IGBITCPU_HPP
