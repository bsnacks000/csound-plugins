#include "blep.h"

#include <dsp/pblep.h>
#include <dsp/shape.h>
#include "csdl.h"

int ssaw_init(CSOUND* csound, ssaw* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    float iphs = clamp(*obj->i_phase, 0.0, 1.0);
    blepsaw_init(&obj->osc, 1.0, iphs, sr);

    return OK;
}

int ssaw_vector(CSOUND* csound, ssaw* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);
    blepsaw_tick_block(&obj->osc, obj->a_out, obj->a_freq, 0, nsmps);
    return OK;
}

int ssqr_init(CSOUND* csound, ssqr* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    float iphs = clamp(*obj->i_phase, 0.0, 1.0);
    blepsqr_init(&obj->osc, 1.0, 0.5, iphs, sr);

    return OK;
}

int ssqr_vector(CSOUND* csound, ssqr* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);
    blepsqr_tick_block(&obj->osc, obj->a_out, obj->a_freq, obj->a_duty, 0, nsmps);
    return OK;
}
