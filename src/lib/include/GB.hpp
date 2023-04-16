#ifndef LOTUS_GB_GB_HPP
#define LOTUS_GB_GB_HPP
#include <memory>
#include "IGB.hpp"
#include "GBCPU.hpp"
#include "IMemoryAccess.hpp"

namespace LOTUSGB {
// A GB object handle all business logic, use GUI/Audio as plugins.
// Hold object's life cycles

class GB: public IGB {
public:
    GB(unique_ptr<IMemoryAccess> &&iMMU, unique_ptr<ICPU> &iCPU);
    ~GB();
    void reset(); // reset GB states, load BIOS or game ... etc.
    void step(); // step until draw one frame

private:
    unique_ptr<IMemoryAccess> upMMU = nullptr;
    unique_ptr<ICPU> upCPU = nullptr;
};


}

#endif // LOTUS_GB_GB_HPP