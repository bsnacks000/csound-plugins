#ifndef DSP_CSND_PHASOR_H
#define DSP_CSND_PHASOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>
#include <dsp/phasor.h>

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_freq;
    phasor state;
} dphasor;

int dphasor_init(CSOUND* csound, dphasor* obj);

int dphasor_vector(CSOUND* csound, dphasor* obj);

#ifdef __cplusplus
}
#endif

#endif
