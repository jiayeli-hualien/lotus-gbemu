#ifndef LOTUSGB_MEMORY_HPP
#define LOTUSGB_MEMORY_HPP
#include <cstddef>
#include <vector>
#include "IMemoryAccess.hpp"

namespace LOTUSGB {

class Memory : public IMemoryAccess {
public:
    Memory();
    Memory(size_t ramSize);
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);
    virtual void resize(size_t size);
protected:
    std::vector<uint8_t> mem;
};

}


#endif // LOTUSGB_MEMORY_HPP