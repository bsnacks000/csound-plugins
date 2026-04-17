#ifndef DSP_CSND_SVF_H
#define DSP_CSND_SVF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <dsp/svf.h>
#include "csdl.h"

typedef struct svflp_ {
    OPDS h;
    MYFLT *a_out, *a_in, *a_freq, *a_q, *a_drive;
    svf state;
} svflp;

int svflp_init(CSOUND* csound, svflp* obj);
int svflp_vector(CSOUND* csound, svflp* obj);

// multimode
// NOTE: leaving off Allpass out for now
typedef struct svfmm_ {
    OPDS h;
    MYFLT *a_lp_out, *a_hp_out, *a_bp_out, *a_bs_out, *a_in, *a_freq, *a_q,
        *a_drive;  // aaaa-aaaa
    svf state;
} svfmm;

int svfmm_init(CSOUND* csound, svfmm* obj);
int svfmm_vector(CSOUND* csound, svfmm* obj);

#ifdef __cplusplus
}
#endif

#endif
