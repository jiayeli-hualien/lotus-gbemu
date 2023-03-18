#ifndef LOTUSGB_IDECODER_HPP
#define LOTUSGB_IDECODER_HPP
#include <fstream>
#include "Iinstruction.hpp"
#include "instruction.hpp"

namespace LOTUSGB {

class IDecoder {
public:
    virtual ~IDecoder() {};
    virtual IInstruction* decode(const uint8_t &opcode) = 0;
};

}

#endif // #ifndef LOTUSGB_IDECODER_HPP
