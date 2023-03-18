/*
Lotus GB detail implementation.
implement GB related interfaces to emu a GB.
*/
#ifndef LOTUS_GB_HPP
#define LOTUS_GB_HPP
#include<memory>
#include "ICPU.hpp"
#include "IGBITCPU.hpp"
#include "ILotusGBIT.hpp"
#include "IMemoryGBIT.hpp"
#include "gb_reg.hpp"

namespace LOTUSGB {

using std::shared_ptr;
using std::unique_ptr;

class LotusGBIT : public ILotusGBIT {
public:
    LotusGBIT(unique_ptr<ICPU> &&upCPU, shared_ptr<IMemoryGBIT> spMMU);
    ~LotusGBIT();
    void init(size_t instruction_mem_size, uint8_t *instruction_mem);
    void getState(LotusGBState *pState);
    void setState(LotusGBState *pState);
    int step(); // step one CPU instruction
private:
    unique_ptr<ICPU> upCPU;
    shared_ptr<IMemoryGBIT> spMMU;
    IGBITCPU *pGBITCPU;
};

}


#endif // LOTUS_GB_HPP
