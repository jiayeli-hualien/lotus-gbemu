#ifndef LOTUSGB_GB_STATE_FOR_GBIT_H
#define LOTUSGB_GB_STATE_FOR_GBIT_H

#include <stdint.h>
#include <stdbool.h>

// re-write struct to decouple GBIT, provide C call C++ interface
// TODO: find a better way.

typedef struct LotusGBMemAccess LotusGBMemAccess;
struct LotusGBMemAccess {
    int type;
    uint16_t address;
    uint8_t value;
};

typedef struct LotusGBState LotusGBState;
struct LotusGBState {
    uint8_t f, a, c, b, e, d, l, h;
    uint16_t sp, pc;
    bool is_halted;
    bool is_interrupts_master_enabled;

    int num_accesses;
    struct LotusGBMemAccess mem_accesses[16];
};

#endif // LOTUSGB_GB_STATE_FOR_GBIT_H
