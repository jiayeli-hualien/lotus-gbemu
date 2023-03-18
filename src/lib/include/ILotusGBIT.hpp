/*
Lotus GB C++ interface for GBIT
*/
#ifndef LOTUSGB_LOTUS_GBIT_INTERFACE
#define LOTUSGB_LOTUS_GBIT_INTERFACE
#include "lotus_gbit_state.h"


class ILotusGBIT;
using pILotusGBIT = ILotusGBIT*;

class ILotusGBIT {
public:
    virtual ~ILotusGBIT() {};
    virtual void init(size_t instruction_mem_size, uint8_t *instruction_mem) = 0;
    virtual void getState(LotusGBState *pState) = 0;
    virtual void setState(LotusGBState *pState) = 0;
    virtual int step() = 0;
};

#endif // LOTUSGB_LOTUS_GBIT_INTERFACE
