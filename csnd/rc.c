#include "rc.h"
#include "csdl.h"
#include "dsp/rc.h"

int rclp_init(CSOUND* csound, rclp* obj) {
    (void) csound;
    float sr = GetLocalSr(&obj->h);

    rc_one_pole_init(&obj->state, 440.0, sr);
    return OK;
}

int rclp_vector(CSOUND* csound, rclp* obj) {
    (void) csound;
    u_int32_t nsamps = GetLocalKsmps(&obj->h);
    rc_one_pole_tick_block(&obj->state, obj->a_in, obj->a_out, NULL, NULL, obj->a_freq,
                           0, nsamps);
    return OK;
}

int rcmm_init(CSOUND* csound, rcmm* obj) {
    (void) csound;
    float sr = GetLocalSr(&obj->h);

    rc_one_pole_init(&obj->state, 440.0, sr);
    return OK;
}

int rcmm_vector(CSOUND* csound, rcmm* obj) {
    (void) csound;
    u_int32_t nsamps = GetLocalKsmps(&obj->h);
    rc_one_pole_tick_block(&obj->state, obj->a_in, obj->a_lp_out, obj->a_hp_out,
                           obj->a_ap_out, obj->a_freq, 0, nsamps);
    return OK;
}

int rcladder_init(CSOUND* csound, rcladder* obj) {
    (void) csound;
    float sr = GetLocalSr(&obj->h);

    rc_ladder_init(&obj->state, 440.0, 0.5, sr);
    return OK;
}

int rcladder_vector(CSOUND* csound, rcladder* obj) {
    (void) csound;
    float nsmps = GetLocalKsmps(&obj->h);

    rc_ladder_tick_block(&obj->state, obj->a_out, obj->a_in, obj->a_freq, obj->a_q, 0,
                         nsmps);
    return OK;
}
