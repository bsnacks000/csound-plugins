/**
 * @brief
 * various delay implementations
 */
#ifndef DSP_CSND_DDELAY_H
#define DSP_CSND_DDELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>
#include <dsp/delay.h>

// standard vdelay using vectorized del.h
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *a_ms, *a_fb, *i_delmax_sec;  // a aaai
    AUXCH buf, yn, dn;
    size_t buf_sz;
    delay_line dl;
    float nsmps_per_ms;
} ddelay2;

int ddelay2_init(CSOUND* csound, ddelay2* obj);

int ddelay2_vector(CSOUND* csound, ddelay2* obj);

#ifdef __cplusplus
}
#endif

#endif
