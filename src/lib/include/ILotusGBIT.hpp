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
    virtual void getState(LotusGBState *pState) = 0;
};

#endif // LOTUSGB_LOTUS_GBIT_INTERFACE
