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

struct DecodeRule {
    string instName;
    uint8_t mask = 0;
    uint8_t matchValue = 0;
};

static string trim(const string &str) {
    static const string SPACE_CHARS(" \t\n\v\f\r");
    return str.substr(str.find_first_not_of(SPACE_CHARS),
                      str.find_last_not_of(SPACE_CHARS)+1);
}

static vector<DecodeRule>
readCsvDecodeTable(std::ifstream &finDecodeTable) {
    vector<DecodeRule> rules;
    string line;
    istringstream iss;
    getline(finDecodeTable, line); // ignore first line
    while(getline(finDecodeTable, line)) {
        line = trim(line);
        if (line.empty())
            continue;
        iss.str(line);
        iss.clear();
        string col[3];
        int cnt = 0;
        while(getline(iss, col[cnt], ',') && cnt < 3) {
            col[cnt] = trim(col[cnt]);
            cnt++;
        }
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

static bool getSubFuncList(vector<Instruction::FUNC_TYPE> &subFuncList,
                           istringstream &iss, int memCycle,
                           subFunMapType &subFuncMap) {
    string token;
    for (int i=0; i<memCycle; i++) {
        if (!getline(iss, token, ','))
            return false;
        token = trim(token);
        auto it = subFuncMap.find(token);
        if (it == subFuncMap.end()) {
            std::cerr << "unkown sub function name: \""
                << token << "\"" << std::endl;
            return false;
        }
        subFuncList.emplace_back(it->second);
    }
    if (getline(iss, token, ',')) {
        std::cerr << "too may subfunction: \""
            << token << "\"" << std::endl;
        return false;
    }
    return true;
}

static unordered_map<string, int>
readCsvInstTable(std::vector<Instruction> &instList, std::ifstream &finInstSet, subFunMapType &subFuncMap) {
    unordered_map<string, int> instIdx;
    string line, token;
    istringstream iss;
    getline(finInstSet, line); // ignore first line
    while(getline(finInstSet, line)) {
        line = trim(line);
        if (line.empty())
            continue;
        iss.str(line); // reuse iss
        iss.clear();
        string instName;
        if(!getline(iss, instName, ',')) {
            std::cerr << "invalid row:" << line << std::endl;
            continue;
        }
        instName = trim(instName);
        if(!getline(iss, token, ',')) {
            std::cerr << "invalid row:" << line << std::endl;
            continue;
        }
        token = trim(token);
        // TODO: format check
        int memCycle = std::stoi(token, nullptr, 10);
        vector<Instruction::FUNC_TYPE> subFuncList;
        if (!getSubFuncList(subFuncList, iss, memCycle, subFuncMap)) {
            std::cerr << "invalid row:" << line << std::endl;
            continue;
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

void Decoder::showLUT() {
    for (int opcode=0; opcode<256; opcode++) {
        auto pInst = reinterpret_cast<Instruction*>(decode(opcode));
        std::cerr << "LUT[" << std::hex << opcode << "] =";
        if(pInst)
            std::cerr << " " << pInst->getName();
        std::cerr << std::endl;
    }
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
    showLUT();
}

IInstruction* Decoder::decode(const uint8_t &opcode) {
    if (lutInstIdx[opcode]>=instList.size()) {
        // TODO: verbose level
        return nullptr;
    }
    auto ret = &(instList[lutInstIdx[opcode]]);
    if (!ret) {
        std::cerr << "faild to get instruction of opcode" << std::hex << (int) opcode << std::endl;
    }
    return ret;
}


}