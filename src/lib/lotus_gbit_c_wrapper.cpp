#include <fstream>
#include "include/lotus_gbit_c_wrapper.h"
#include "include/ILotusGBIT.hpp"
#include "include/lotusGB.hpp"
#include "include/GBCPU.hpp"
#include "include/memory.hpp"
#include "include/memoryGBIT.hpp"
#include "include/decoder.hpp"

using LOTUSGB::LotusGB;
using LOTUSGB::GBCPU;
using LOTUSGB::Memory;
using LOTUSGB::MemoryGBIT;
using LOTUSGB::Decoder;
using std::ifstream;

pILotusGBIT lotusGBIT_Create() {
    // TODO: factory
    // TODO: delete objects
    MemoryGBIT *pMockMMU = new MemoryGBIT();

    // TODO: filepath from config
    ifstream finDecodeTable, finInstructionTable;
    std::string pathDecodeTable = "./decode_table.csv";
    std::string pathInstructionTable = "./instruction_table.csv";
    finDecodeTable.open(pathDecodeTable);
    finInstructionTable.open(pathInstructionTable);
    if (!finDecodeTable) {
        std::cerr << "file\""<< pathDecodeTable <<"\" not found" << std::endl;
        return nullptr;
    }
    if (!finInstructionTable) {
        std::cerr << "file\""<< pathInstructionTable <<"\" not found" << std::endl;
        return nullptr;
    }
    Decoder *pDecoder = new Decoder(finDecodeTable, finInstructionTable);

    GBCPU *pCPU = new GBCPU(pMockMMU, pDecoder);
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
