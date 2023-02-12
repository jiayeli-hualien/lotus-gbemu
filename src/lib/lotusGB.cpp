#include "include/lotusGB.hpp"

namespace LOTUSGB {

Reg LotusGB::getReg() {
    return this->reg;
}

void LotusGB::getState(LotusGBState *pState) {
    // TODO: complete this function
    reg.getRefA() = 0x88; // developing, just try
    pState->a = reg.getRefA();
}

}
