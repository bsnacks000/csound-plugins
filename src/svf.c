#include "svf.h"

int svflp_init(CSOUND* csound, svflp* obj) {
    (void) csound;
    float sr = GetLocalSr(&obj->h);

    svf_init(&obj->state, 440.0, 0.707, 0.0, sr);
    return OK;
}

int svflp_vector(CSOUND* csound, svflp* obj) {
    (void) csound;
    u_int32_t nsamps = GetLocalKsmps(&obj->h);

    svf_tick_block(&obj->state, obj->a_out, NULL, NULL, NULL, NULL, obj->a_in,
                   obj->a_freq, obj->a_q, obj->a_drive, 0, nsamps);
    return OK;
}

int svfmm_init(CSOUND* csound, svfmm* obj) {
    (void) csound;
    float sr = GetLocalSr(&obj->h);
    svf_init(&obj->state, 440.0, 0.707, 0.0, sr);
    return OK;
}

int svfmm_vector(CSOUND* csound, svfmm* obj) {
    (void) csound;
    u_int32_t nsamps = GetLocalKsmps(&obj->h);

    svf_tick_block(&obj->state, obj->a_lp_out, obj->a_hp_out, obj->a_bp_out,
                   obj->a_bs_out, NULL, obj->a_in, obj->a_freq, obj->a_q, obj->a_drive,
                   0, nsamps);
    return OK;
}
