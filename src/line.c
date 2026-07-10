#include "line.h"

int lline_init(CSOUND* csound, lline* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    line_init(&obj->l, *obj->i_start, *obj->i_end, *obj->i_dur, sr);
    return OK;
}

int lline_vector(CSOUND* csound, lline* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    line_tick_block(&obj->l, obj->a_out, 0, nsmps);
    return OK;
}

int ssampi_init(CSOUND* csound, ssampi* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    sampi_init(&obj->s, *obj->i_start, *obj->i_end, *obj->i_dur, sr);
    return OK;
}

int ssampi_vector(CSOUND* csound, ssampi* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    sampi_tick_block(&obj->s, obj->a_out, obj->a_in, obj->a_dur, obj->a_gate,
                     obj->a_gate_thresh, 0, nsmps);
    return OK;
}
