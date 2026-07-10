#include "lag.h"
#include "csdl.h"
#include "dsp/lag.h"

int llag_init(CSOUND* csound, llag* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);
    lag_init(&obj->ctrl, 0.1, sr);
    return OK;
}

int llag_vector(CSOUND* csound, llag* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);
    lag_tick_block(&obj->ctrl, obj->a_out, obj->a_in, obj->t_sec, 0, nsmps);
    return OK;
}
