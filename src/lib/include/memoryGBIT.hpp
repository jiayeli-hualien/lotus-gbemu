#ifndef LOTUSGB_MEMORY_GBIT_HPP
#define LOTUSGB_MEMORY_GBIT_HPP
#include <cstddef>
#include <array>
#include "memory.hpp"
#include "IMemoryGBIT.hpp"

namespace LOTUSGB {

class MemoryGBIT : public IMemoryGBIT {
/*
Mock memory/MMU for GBIT
Want to test class memory (if I can), inherits GB memory and extends
TODO: Inheritance MMU instead of Memory if MMU is implement (GBIT tests a MMU)
*/
public:
    MemoryGBIT();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);
    int getMemoryState(
            LotusGBMemAccess accessRec[LOTUSGB_GBIT_MEM_ACCESS_NUM]);
    void setMemoryState(int num_mem_accesses,
            LotusGBMemAccess accessRec[LOTUSGB_GBIT_MEM_ACCESS_NUM]);
    void setInsturctionMem(size_t instructionMemSize,
            uint8_t *instruction_mem);
    void resize(size_t size);
private:
    size_t instructionMemSize = 0;
    uint8_t *pInstructionMem = nullptr;
    int numMemAccess = 0;
    LotusGBMemAccess memAccessRecord[LOTUSGB_GBIT_MEM_ACCESS_NUM];
};

}


#endif // LOTUSGB_MEMORY_HPP