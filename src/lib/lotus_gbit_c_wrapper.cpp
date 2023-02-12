#include "include/lotus_gbit_c_wrapper.h"
#include "include/ILotusGBIT.hpp"
#include "include/lotusGB.hpp"


using LOTUSGB::LotusGB;

pILotusGBIT lotusGBIT_Create() {
    return new LotusGB();
}

void lotusGBIT_GetStatus(pILotusGBIT self, LotusGBState *pState) {
    self->getState(pState);
}
