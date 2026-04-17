#include "curve.h"

int ccurve_init(CSOUND* csound, ccurve* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    curve_init(&obj->crv, *obj->i_start, *obj->i_end, *obj->i_dur, *obj->i_factor, sr);
    return OK;
}

int ccurve_vector(CSOUND* csound, ccurve* obj) {
    (void) csound;
    (void) obj;

    uint32_t nsmps = GetLocalKsmps(&obj->h);

    curve_tick_block(&obj->crv, obj->a_out, 0, nsmps);

    return OK;
}
