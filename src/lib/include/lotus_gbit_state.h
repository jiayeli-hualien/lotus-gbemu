#ifndef LOTUSGB_GB_STATE_FOR_GBIT_H
#define LOTUSGB_GB_STATE_FOR_GBIT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define GBIT_MEM_ACCESS_WRITE 1

// re-write struct to decouple GBIT, provide C call C++ interface
// TODO: find a better way.

typedef struct LotusGBMemAccess LotusGBMemAccess;
struct LotusGBMemAccess {
    int type;
    uint16_t address;
    uint8_t value;
};

#define LOTUSGB_GBIT_MEM_ACCESS_NUM 16
typedef struct LotusGBState LotusGBState;
struct LotusGBState {
    uint8_t f, a, c, b, e, d, l, h;
    uint16_t sp, pc;
    bool is_halt;
    bool interrupts_master_enable;

    int num_accesses;
    struct LotusGBMemAccess mem_accesses[LOTUSGB_GBIT_MEM_ACCESS_NUM];
};

#endif // LOTUSGB_GB_STATE_FOR_GBIT_H
