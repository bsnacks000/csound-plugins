#ifndef DSP_CSND_RC_H
#define DSP_CSND_RC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <dsp/rc.h>
#include "csdl.h"

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *a_freq;
    rc_one_pole state;
} rclp;

int rclp_init(CSOUND* csound, rclp* obj);

int rclp_vector(CSOUND* csound, rclp* obj);

typedef struct {
    OPDS h;
    MYFLT *a_lp_out, *a_hp_out, *a_ap_out, *a_in, *a_freq;  // aaa-aa
    rc_one_pole state;
} rcmm;

int rcmm_init(CSOUND* csound, rcmm* obj);

int rcmm_vector(CSOUND* csound, rcmm* obj);

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *a_freq, *a_q;  // aa-aaa
    rc_ladder state;
} rcladder;

int rcladder_init(CSOUND* csound, rcladder* obj);

int rcladder_vector(CSOUND* csound, rcladder* obj);

#ifdef __cplusplus
}
#endif

#endif
