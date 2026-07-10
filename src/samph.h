#ifndef DSP_CSND_SAMPH_H
#define DSP_CSND_SAMPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>
#include <dsp/samph.h>

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *a_gate, *a_gate_thresh;  // a-aaa
    samph s;
} ssamph;

int ssamph_init(CSOUND* csound, ssamph* obj);

int ssamph_vector(CSOUND* csound, ssamph* obj);

#ifdef __cplusplus
}
#endif

#endif
