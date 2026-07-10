#include "maths.h"
#include <dsp/maths.h>

int mult_vector(CSOUND* csound, mult* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);
    mult_block(obj->a_out, obj->a_in1, obj->a_in2, 0, nsmps);
    return OK;
}
