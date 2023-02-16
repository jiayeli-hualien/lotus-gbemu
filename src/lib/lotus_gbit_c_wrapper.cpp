#include "include/lotus_gbit_c_wrapper.h"
#include "include/ILotusGBIT.hpp"
#include "include/lotusGB.hpp"
#include "include/GBCPU.hpp"
#include "include/memory.hpp"
#include "include/memoryGBIT.hpp"

using LOTUSGB::LotusGB;
using LOTUSGB::GBCPU;
using LOTUSGB::Memory;
using LOTUSGB::MemoryGBIT;

pILotusGBIT lotusGBIT_Create() {
    // TODO: factory
    // TODO: delete objects
    MemoryGBIT *pMockMMU = new MemoryGBIT();
    GBCPU *pCPU = new GBCPU(pMockMMU);
    return new LotusGB(pCPU, pMockMMU);
}

void lotusGBIT_Init(pILotusGBIT self, size_t instruction_mem_size, uint8_t *instruction_mem) {
    self->init(instruction_mem_size, instruction_mem);
}

void lotusGBIT_GetStatus(pILotusGBIT self, LotusGBState *pState) {
    self->getState(pState);
}

void lotusGBIT_SetStatus(pILotusGBIT self, LotusGBState *pState) {
    self->setState(pState);
}

int lotusGBIT_Step(pILotusGBIT self) {
    return self->step();
}
