#ifndef LOTUSGB_DECODER_HPP
#define LOTUSGB_DECODER_HPP
#include <fstream>
#include "Iinstruction.hpp"
#include "instruction.hpp"

namespace LOTUSGB {

class Decoder {
    // Instruct factory, get instruction object from opcode
    // TODO: refactor by abstraction factory pattern
public:
    Decoder(std::ifstream &finDecodeTable, std::ifstream &finInstSet);
    IInstruction* decode(const uint8_t &opcode);
private:
    void showLUT();
    std::vector<Instruction> instList;
    uint8_t lutInstIdx[256];
};

}

#endif // #ifndef LOTUSGB_DECODER_HPP