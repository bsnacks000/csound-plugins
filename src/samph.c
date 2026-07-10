#include "samph.h"
#include "csdl.h"
#include "dsp/samph.h"

int ssamph_init(CSOUND* csound, ssamph* obj) {
    (void) csound;
    samph_init(&obj->s, 1.0);
    return OK;
}

int ssamph_vector(CSOUND* csound, ssamph* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    samph_tick_block(&obj->s, obj->a_out, obj->a_in, obj->a_gate, obj->a_gate_thresh, 0,
                     nsmps);

    return OK;
}
