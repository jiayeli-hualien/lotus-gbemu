#include "include/decoder.hpp"
#include "include/instSubFunc.hpp"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::unordered_map;
using std::vector;
using std::istringstream;
using std::getline;

namespace LOTUSGB {

using subFunMapType = unordered_map<string, Instruction::FUNC_TYPE>;
static subFunMapType getSubFuncMap() {
    subFunMapType map;
    // TODO: codegen?
    map["subFuncNOP"] = subFuncNOP();
    map["subFuncLDRR"] = subFuncLDRR();
    map["subFuncMemRead"] = subFuncMemRead();
    map["subFuncLDR_IMMD"] = subFuncLDR_IMMD();
    return map;
}

struct DecodeRule {
    string instName;
    uint8_t mask = 0;
    uint8_t matchValue = 0;
};

vector<DecodeRule> readCsvDecodeTable(std::ifstream &finDecodeTable) {
    vector<DecodeRule> rules;
    string line;
    istringstream iss;
    getline(finDecodeTable, line); // ignore first line
    while(getline(finDecodeTable, line)) {
        iss.str(line);
        string col[3];
        int cnt = 0;
        while(getline(iss, col[cnt], ',') && cnt < 3)
            cnt++;
        if (cnt<3) {
            std::cerr << "invalid row:" << line << std::endl;
            continue;
        }
        // TODO: check file format
        DecodeRule rule;
        rule.instName = col[0];
        rule.mask = (uint8_t) std::stoul(col[1], nullptr, 16);
        rule.matchValue = (uint8_t) std::stoul(col[2], nullptr, 16);
        rules.emplace_back(rule);
    }
    return rules;
}

static unordered_map<string, int>
readCsvInstTable(std::vector<Instruction> &instList, std::ifstream &finInstSet, subFunMapType &subFuncMap) {
    unordered_map<string, int> instIdx;
    string line, token;
    istringstream iss;
    getline(finInstSet, line); // ignore first line
    while(getline(finInstSet, line)) {
        iss.str(line);
        string instName;
        if(!getline(iss, instName, ',')) {
            std::cerr << "invalid row:" << line << std::endl;
            continue;
        }
        if(!getline(iss, token, ',')) {
            std::cerr << "invalid row:" << line << std::endl;
            continue;
        }
        // TODO: format check
        int memCycle = std::stoi(token, nullptr, 10);
        vector<Instruction::FUNC_TYPE> subFuncList;
        for (int i=0; i<memCycle; i++) {
            if (!getline(iss, token, ','))
                break;
            subFuncList.emplace_back(subFuncMap[token]);
        }
        if ((int)subFuncList.size() != memCycle) {
            std::cerr << "invalid row:" << line << std::endl;
            continue;
        }
        instIdx[instName] = instList.size();
        instList.emplace_back(instName, memCycle, move(subFuncList));
    }
    return instIdx;
}

Decoder::Decoder(std::ifstream &finDecodeTable, std::ifstream &finInstSet) {
    subFunMapType subFuncMap = getSubFuncMap();
    vector<DecodeRule> rules = readCsvDecodeTable(finDecodeTable);
    unordered_map<string, int> instIdx = readCsvInstTable(instList, finInstSet, subFuncMap);

    // Build LUT
    for (int opcode=0; opcode<256; opcode++) {
        lutInstIdx[opcode] = instList.size(); // init by an invalid value
        const int N = (int) rules.size();
        for (int r=0; r < N; r++) {
            const DecodeRule &rule = rules[r];
            if ((opcode&rule.mask) == rule.matchValue) {
                lutInstIdx[opcode] = instIdx[rule.instName];
                break;
            }
        }
    }
}

IInstruction* Decoder::decode(const uint8_t &opcode) {
    return &(instList[lutInstIdx[opcode]]);
}


}