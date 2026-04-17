#ifndef DSP_CSND_DISTORT_H
#define DSP_CSND_DISTORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>
#include <dsp/balance.h>
#include <dsp/ftable/ftable.h>
#include <dsp/tabread.h>
#include <dsp/xfade.h>

/**
 * @brief chebyshev polynomial saw generator.
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in;

    tabread tr;  // read cheby table
} chebsaw;

int chebsaw_tab_init(CSOUND* csound);
int chebsaw_init(CSOUND* csound, chebsaw* obj);
int chebsaw_vector(CSOUND* csound, chebsaw* obj);

typedef void (
    *saturator_func)(float* out, float* x, float* amt, uint32_t start, uint32_t nsmps);

/**
 * @brief multi mode saturator
 * 0=hard_clip
 * 1=exp_clip
 * 2=tanh_clip
 * 3=atan_clip
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *a_amt, *i_mode;

    saturator_func func;
} saturator;

int saturator_init(CSOUND* csound, saturator* obj);
int saturator_vector(CSOUND* csound, saturator* obj);

#ifdef __cplusplus
}
#endif

#endif
