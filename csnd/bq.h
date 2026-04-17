#ifndef DSP_CSND_BQ_H
#define DSP_CSND_BQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <dsp/bq.h>
#include "csdl.h"

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *k_freq, *k_q, *i_type;  // aakki
    bq_resonant state;
} bqres;

int bqres_init(CSOUND* csound, bqres* obj);
int bqres_vector(CSOUND* csound, bqres* obj);

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *k_freq, *i_type;  // aaki
    bq_non_resonant state;
} bqnres;

int bqnres_init(CSOUND* csound, bqnres* obj);
int bqnres_vector(CSOUND* csound, bqnres* obj);

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *k_freq, *k_q, *k_gain, *i_type;
    bq_para_eq state;
} bqpara;

int bqpara_init(CSOUND* csound, bqpara* obj);
int bqpara_vector(CSOUND* csound, bqpara* obj);

#ifdef __cplusplus
}
#endif

#endif
