#include "bq.h"

int bqres_init(CSOUND* csound, bqres* obj) {
    (void) csound;
    float sr = GetLocalSr(&obj->h);
    int t = (bq_resonant_type) *obj->i_type;
    bq_resonant_init(&obj->state, t, 440.0, 0.707, sr);
    return OK;
}

int bqres_vector(CSOUND* csound, bqres* obj) {
    (void) csound;
    u_int32_t nsamps = GetLocalKsmps(&obj->h);
    bq_resonant_tick_block(&obj->state, obj->a_out, obj->a_in, *obj->k_freq, *obj->k_q,
                           0, nsamps);

    return OK;
}

int bqnres_init(CSOUND* csound, bqnres* obj) {
    (void) csound;
    float sr = GetLocalSr(&obj->h);
    int t = (bq_non_resonant_type) *obj->i_type;
    bq_non_resonant_init(&obj->state, t, 440.0, sr);
    return OK;
}

int bqnres_vector(CSOUND* csound, bqnres* obj) {
    (void) csound;
    u_int32_t nsamps = GetLocalKsmps(&obj->h);
    bq_non_resonant_tick_block(&obj->state, obj->a_out, obj->a_in, *obj->k_freq, 0,
                               nsamps);

    return OK;
}

int bqpara_init(CSOUND* csound, bqpara* obj) {
    (void) csound;
    float sr = GetLocalSr(&obj->h);
    int t = (bq_para_eq_type) *obj->i_type;
    bq_para_eq_init(&obj->state, t, 440.0, 0.707, 0.0, sr);
    return OK;
}

int bqpara_vector(CSOUND* csound, bqpara* obj) {
    (void) csound;
    u_int32_t nsamps = GetLocalKsmps(&obj->h);
    bq_para_eq_tick_block(&obj->state, obj->a_out, obj->a_in, *obj->k_freq, *obj->k_q,
                          *obj->k_gain, 0, nsamps);
    return OK;
}
