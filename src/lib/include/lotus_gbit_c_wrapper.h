#ifndef LOTUSGB_GBIT_C_WRAPPER_H
#define LOTUSGB_GBIT_C_WRAPPER_H
#include "lotus_gbit_state.h"
#ifdef __cplusplus
#include "ILotusGBIT.hpp"
#else // __cplusplus

// Dummy C type and pointer
typedef struct ILotusGBIT ILotusGBIT;
typedef ILotusGBIT *pILotusGBIT;

#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

pILotusGBIT lotusGBIT_Create();
void lotusGBIT_GetStatus(pILotusGBIT self, LotusGBState *state);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LOTUSGB_GBIT_C_WRAPPER_H
