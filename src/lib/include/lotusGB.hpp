/*
Lotus GB detail implementation.
implement GB related interfaces to emu a GB.
*/
#ifndef LOTUS_GB_HPP
#define LOTUS_GB_HPP
#include "ICPU.hpp"
#include "ILotusGBIT.hpp"
#include "IMemoryGBIT.hpp"
#include "gb_reg.hpp"

namespace LOTUSGB {

class LotusGB : public ILotusGBIT {
public:
    LotusGB(ICPU *pCPU, IMemoryGBIT *pMMU, IMemoryAccess *pInstructionMemory);
    void init(size_t instruction_mem_size, uint8_t *instruction_mem);
    void getState(LotusGBState *pState);
    void setState(LotusGBState *pState);
    int step(); // step one CPU instruction
private:
    // TODO: only access via interface
    ICPU *pCPU;
    IMemoryGBIT *pMMU; 
    IMemoryAccess *pInstructionMemory;
};

}


#endif // LOTUS_GB_HPP
