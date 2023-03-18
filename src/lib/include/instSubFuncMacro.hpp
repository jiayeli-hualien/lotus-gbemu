#ifndef LOTUSGB_INST_SUB_FUNC_MACRO_HPP
#define LOTUSGB_INST_SUB_FUNC_MACRO_HPP
#include "Iinstruction.hpp"
#include "gb_reg.hpp"

#define SUB_FUNC_PARAMS InstState *pInstState, Reg *pReg
#define SUB_FUNC_ARGS pInstState, pReg

#if INST_USE_FUNC_OBJ

#define SUB_FUNC_OP operator () (SUB_FUNC_PARAMS)

#define SUB_FUNC_DECLARE(x) \
struct subFunc##x {\
SUB_FUNC_OP_RET SUB_FUNC_OP;\
};

#define SUB_FUNC_IMPL(x) SUB_FUNC_OP_RET subFunc##x::SUB_FUNC_OP

#define MAP_ENTRY(name) (map[#name]=subFunc##name())

#else // INST_USE_FUNC_OBJ

#define SUB_FUNC_IMPL(x) SUB_FUNC_OP_RET subFunc##x(SUB_FUNC_PARAMS)
#define SUB_FUNC_DECLARE(x) SUB_FUNC_OP_RET subFunc##x(SUB_FUNC_PARAMS)
#define MAP_ENTRY(name) (map[#name]=subFunc##name)

#endif // INST_USE_FUNC_OBJ

// int for leverage int promotion
static constexpr int BYTE_MASK = 0xFF;
static constexpr int HALF_MASK = 0xF;
#define GET_LSB(x) ((uint8_t)((x) & BYTE_MASK))
#define GET_LSB_INT(x) ((x) & BYTE_MASK)
#define GET_HALF_INT(x) ((x) & HALF_MASK)
static constexpr int HALF_UPPER_BOUND = 0xF;
static constexpr int BYTE_UPPER_BOUND = 0xFF;
#define HALF_OP_HELPER(x, op, y) (GET_HALF_INT((x)) op GET_HALF_INT((y)))

#endif // LOTUSGB_INST_SUB_FUNC_MACRO_HPP
