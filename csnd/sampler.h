#ifndef DSP_CSND_SAMPLER_H
#define DSP_CSND_SAMPLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>

#include <dsp/ftable/ftable.h>
#include <dsp/phasor.h>
#include <dsp/tabread.h>

/**
 * simple looping sampler with speed control
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_speed, *i_skip, *i_fn;
    FUNC* ftp;  // the ftable pointer

    // aux mem for internals
    AUXCH freq;
    AUXCH idxs;

    float dur;  // the sample dur (buf_sz / sr)
    ftable wt;
    phasor ph;
    tabread tab;
} sampler;

int sampler_init(CSOUND* csound, sampler* obj);

int sampler_vector(CSOUND* csound, sampler* obj);

#ifdef __cplusplus
}
#endif

#endif
