#ifndef LOTUSGB_IMEMORY_GBIT_HPP
#define LOTUSGB_IMEMORY_GBIT_HPP

#include "IMemoryAccess.hpp"
#include "lotus_gbit_state.h"

namespace LOTUSGB {

class IMemoryGBIT: public IMemoryAccess {
public:
    virtual int
        getMemoryState(
            LotusGBMemAccess accessRec[LOTUSGB_GBIT_MEM_ACCESS_NUM]) = 0;
    virtual void
        setMemoryState(int num_mem_accesses,
            LotusGBMemAccess accessRec[LOTUSGB_GBIT_MEM_ACCESS_NUM]) = 0;
    virtual void
        setInsturctionMem(size_t instructionMemSize,
                            uint8_t *instruction_mem) = 0;
};

}
#endif // LOTUSGB_IMEMORY_HPP