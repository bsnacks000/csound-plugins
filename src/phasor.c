#include "phasor.h"

int dphasor_init(CSOUND* csound, dphasor* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    phasor_init(&obj->state, 1.0, 0.0, sr);
    return OK;
}

int dphasor_vector(CSOUND* csound, dphasor* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    phasor_tick_block(&obj->state, obj->a_out, obj->a_freq, 0, nsmps);

    return OK;
}
