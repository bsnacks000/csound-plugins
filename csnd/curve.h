#ifndef DSP_CSND_CURVE_H
#define DSP_CSND_CURVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>
#include <dsp/curve.h>

typedef struct {
    OPDS h;
    MYFLT *a_out, *i_start, *i_dur, *i_end, *i_factor;
    curve crv;
} ccurve;

int ccurve_init(CSOUND* csound, ccurve* obj);

int ccurve_vector(CSOUND* csound, ccurve* obj);

#ifdef __cplusplus
}
#endif

#endif
