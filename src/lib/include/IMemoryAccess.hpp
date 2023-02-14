#ifndef LOTUSGB_IMEMORY_ACCESS_HPP
#define LOTUSGB_IMEMORY_ACCESS_HPP

#include <cstdint>

namespace LOTUSGB {

class IMemoryAccess {
    // Common interfaoce for memory and MMU
    // TODO: memory clock definitions
public:
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t value) = 0;
    virtual void resize(size_t size) = 0;
};

}
#endif // LOTUSGB_IMEMORY_HPP