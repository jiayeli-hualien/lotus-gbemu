#ifndef LOTUS_GB_IGB_HPP
#define LOTUS_GB_IGB_HPP

namespace LOTUSGB {
// A GB object handle all business logic, use GUI/Audio as plugins.

class IGB {
public:
    virtual ~IGB() {};
    virtual void reset() = 0; // reset GB states, load BIOS or game ... etc.
    virtual void step() = 0; // step until draw one frame
};


}

#endif // LOTUS_GB_IGB_HPP