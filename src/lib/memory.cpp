#include "include/memory.hpp"

namespace LOTUSGB {

Memory::Memory() {}

Memory::Memory(size_t ramSize = 8*1024) :mem(ramSize, 0) {
}

uint8_t Memory::read(uint16_t addr) {
    return mem[addr];
}

void Memory::write(uint16_t addr, uint8_t value) {
    mem[addr] = value;
}

void Memory::resize(size_t size) {
    mem.resize(size, 0);
}

}