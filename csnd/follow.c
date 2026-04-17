#include "follow.h"
#include "csdl.h"
#include "dsp/follow.h"
#include "sysdep.h"

int envfol_init(CSOUND* csound, envfol* obj) {
    (void) csound;

    MYFLT sr = GetLocalSr(&obj->h);
    follow_init(&obj->state, 0.1, 0.1, sr);

    return OK;
}

int envfol_vector(CSOUND* csound, envfol* obj) {

    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    follow_peak_tick_block(&obj->state, obj->a_out, obj->a_in, obj->a_atk_ms,
                           obj->a_rel_ms, 0, nsmps);

    return OK;
}
