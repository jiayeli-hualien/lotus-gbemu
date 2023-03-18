#ifndef LOTUSGB_DECODER_HPP
#define LOTUSGB_DECODER_HPP
#include <fstream>
#include "IDecoder.hpp"
#include "Iinstruction.hpp"
#include "instruction.hpp"


namespace LOTUSGB {

class Decoder : public IDecoder {
    // Instruct factory, get instruction object from opcode
    // TODO: refactor by abstraction factory pattern
public:
    Decoder(std::ifstream &finDecodeTable, std::ifstream &finInstSet, const subFunMapType &subFuncMap);
    IInstruction* decode(const uint8_t &opcode);
private:
    void showLUT();
    std::vector<Instruction> instList;
    uint8_t lutInstIdx[256];
};

}

#endif // #ifndef LOTUSGB_DECODER_HPP