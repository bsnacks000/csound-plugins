#ifndef DSP_CSND_FOLLOW_H
#define DSP_CSND_FOLLOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>

#include <dsp/follow.h>

/**
 * @brief envelope follower
 * TODO: add an RMS / PEAK mode switch (assign fptr at init time)
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *a_atk_ms, *a_rel_ms;
    follow state;
} envfol;

int envfol_init(CSOUND* csound, envfol* obj);

int envfol_vector(CSOUND* csound, envfol* obj);

#ifdef __cplusplus
}
#endif

#endif
