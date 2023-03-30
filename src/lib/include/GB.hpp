#ifndef LOTUS_GB_GB_HPP
#define LOTUS_GB_GB_HPP
#include "IGB.hpp"
#include "GBCPU.hpp"
#include "IMemoryAccess.hpp"

namespace LOTUSGB {
// A GB object handle all business logic, use GUI/Audio as plugins.

class GB: public IGB {
public:
    GB();
    ~GB();
    void reset(); // reset GB states, load BIOS or game ... etc.
    void step(); // step until draw one frame
private:
    IMemoryAccess *pMainMMU = nullptr;
    ICPU *pGBCPU = nullptr;
    
};


}

#endif // LOTUS_GB_GB_HPP