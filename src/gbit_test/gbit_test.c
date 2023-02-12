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

int main(int argc, char *argv[]) {
    static struct tester_flags flags = {
    .keep_going_on_mismatch = 0,
    .enable_cb_instruction_testing = 1,
    .print_tested_instruction = 1,
    .print_verbose_inputs = 1,
    };

    return tester_run(&flags, &gbLotusOps);
}

// TODO: remove static variable
static pILotusGBIT gpLotusGB = NULL;



// TODO: implement functions
void lotus_init(size_t instruction_mem_size, uint8_t *instruction_mem) {
    gpLotusGB = lotusGBIT_Create();
}

void lotus_set_state(struct state *state) {

}

void lotus_get_state(struct state *state) {
    LotusGBState lotusGBState;
    lotusGBIT_GetStatus(gpLotusGB, &lotusGBState);

    // TODO: all states
    // TODO: find a better way to decoupling gbit and my repo
    memset(state, 0, sizeof(struct state));
    state->reg8.A = lotusGBState.a;
}

int lotus_step(void) {
    return 0;
}
