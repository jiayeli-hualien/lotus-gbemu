#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tester.h>
#include <lotus_gbit_c_wrapper.h>
#include <lotus_gbit_state.h>

// TODO: how to use my c++ object?
void lotus_init(size_t instruction_mem_size, uint8_t *instruction_mem);
void lotus_set_state(struct state *state);
void lotus_get_state(struct state *state);
int lotus_step(void);

struct tester_operations gbLotusOps = {
    .init = lotus_init,
    .set_state = lotus_set_state,
    .get_state = lotus_get_state,
    .step = lotus_step,
};

// TODO: remove static variable
static pILotusGBIT gpLotusGB = NULL;
int main(int argc, char *argv[]) {
    static struct tester_flags flags = {
    .keep_going_on_mismatch = 0,
    .enable_cb_instruction_testing = 1,
    .print_tested_instruction = 1,
    .print_verbose_inputs = 1,
    };
    gpLotusGB = lotusGBIT_Create();
    return tester_run(&flags, &gbLotusOps);
}

// TODO: implement functions
void lotus_init(size_t instruction_mem_size, uint8_t *instruction_mem) {
    lotusGBIT_Init(gpLotusGB, instruction_mem_size, instruction_mem);
}

void lotus_set_state(struct state *state) {
    LotusGBState lotusGBState = {0};

    lotusGBState.a = state->reg8.A;
    lotusGBState.f = state->reg8.F;
    lotusGBState.pc = state->PC;
    lotusGBState.num_accesses = state->num_mem_accesses;
    memcpy(lotusGBState.mem_accesses, state->mem_accesses, sizeof(state->mem_accesses));

    lotusGBIT_SetStatus(gpLotusGB, &lotusGBState);
}

void lotus_get_state(struct state *state) {
    LotusGBState lotusGBState = {0};
    lotusGBIT_GetStatus(gpLotusGB, &lotusGBState);

    memset(state, 0, sizeof(struct state));
    state->reg8.A = lotusGBState.a;
    state->reg8.F = lotusGBState.f;
    state->PC = lotusGBState.pc;
    state->num_mem_accesses = lotusGBState.num_accesses;
    memcpy(state->mem_accesses, lotusGBState.mem_accesses, sizeof(state->mem_accesses));
}

int lotus_step(void) {
    return lotusGBIT_Step(gpLotusGB);
}
