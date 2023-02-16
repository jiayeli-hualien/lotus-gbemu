#include "include/memoryGBIT.hpp"
#include "include/IMemoryAccess.hpp"
#include <iostream>
#include<cstring>

namespace LOTUSGB {

MemoryGBIT::MemoryGBIT() {
}

uint8_t MemoryGBIT::read(uint16_t addr) {
    if (addr >= instructionMemSize)
        return 0xAA;
    else
        return *(pInstructionMem + (int)addr);
}

void MemoryGBIT::write(uint16_t addr, uint8_t value) {
    LotusGBMemAccess &rec = memAccessRecord[numMemAccess++];
    rec.type = GBIT_MEM_ACCESS_WRITE;
    rec.address = addr;
    rec.value = value;
}

void MemoryGBIT::resize(size_t size) {
}

int MemoryGBIT::getMemoryState(LotusGBMemAccess accessRec[LOTUSGB_GBIT_MEM_ACCESS_NUM]) {
    memset(accessRec, 0, sizeof(LotusGBMemAccess)*LOTUSGB_GBIT_MEM_ACCESS_NUM);
    std::copy(memAccessRecord, memAccessRecord+numMemAccess, accessRec);
    return numMemAccess;
}

void MemoryGBIT::setMemoryState(int num_mem_accesses,
        LotusGBMemAccess accessRec[LOTUSGB_GBIT_MEM_ACCESS_NUM]) {
    numMemAccess = num_mem_accesses;
    std::copy(accessRec, accessRec + LOTUSGB_GBIT_MEM_ACCESS_NUM, memAccessRecord);
}

void MemoryGBIT::setInsturctionMem(size_t instructionMemSize,
        uint8_t *instruction_mem) {
    this->instructionMemSize = instructionMemSize;
    this->pInstructionMem = instruction_mem;
}

}