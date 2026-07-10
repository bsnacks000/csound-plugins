#ifndef DSP_CSOUND_BLEP_H
#define DSP_CSOUND_BLEP_H

#include "csdl.h"
#include "dsp/pblep.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief blepsaw
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_freq, *i_phase;

    blepsaw osc;
} ssaw;

int ssaw_init(CSOUND* csound, ssaw* obj);

int ssaw_vector(CSOUND* csound, ssaw* obj);

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_freq, *a_duty, *i_phase;

    blepsqr osc;
} ssqr;

int ssqr_init(CSOUND* csound, ssqr* obj);

int ssqr_vector(CSOUND* csound, ssqr* obj);

#ifdef __cplusplus
}
#endif

#endif
