/*
Lotus GB detail implementation.
implement GB related interfaces to emu a GB.
*/
#ifndef LOTUS_GB_HPP
#define LOTUS_GB_HPP
#include "ILotusGBIT.hpp"
#include "gb_reg.hpp"

namespace LOTUSGB {

class LotusGB : public ILotusGBIT {
public:
    Reg getReg();
    void getState(LotusGBState *pState);
private:
    Reg reg;
};

}


#endif // LOTUS_GB_HPP
