#include <iostream>
#include <cstddef>
#include <cstdint>
#include "include/instSubFuncCommon.hpp"
#include "include/instruction.hpp"
#include "include/instSubFuncMacro.hpp"
#include "include/gb_reg.hpp"

namespace LOTUSGB {

SUB_FUNC_IMPL(NOP){
    return;
}

SUB_FUNC_IMPL(MemReadIndirectHL) {
    pInstState->memMode = MEM_MODE_READ;
    pInstState->memAddr = pReg->getRefHL();
}

}