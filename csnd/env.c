#include "env.h"
#include "dsp/curve.h"

int ar_line_init(CSOUND* csound, ar_line* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    line_ar_init(&obj->env, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, sr);
    return OK;
}

int ar_line_vector(CSOUND* csound, ar_line* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    line_ar_tick_block(&obj->env, obj->a_out, obj->a_gate, obj->a_gate_thresh,
                       obj->a_start_level, obj->a_atk_sec, obj->a_atk_level,
                       obj->a_rel_sec, obj->a_rel_level, 0, nsmps);
    return OK;
}

int adsr_line_init(CSOUND* csound, adsr_line* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    line_adsr_init(&obj->env, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, sr);
    return OK;
}

int adsr_line_vector(CSOUND* csound, adsr_line* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    line_adsr_tick_block(&obj->env, obj->a_out, obj->a_gate, obj->a_gate_thresh,
                         obj->a_start_level, obj->a_atk_sec, obj->a_atk_level,
                         obj->a_dcy_sec, obj->a_sustain_level, obj->a_rel_sec,
                         obj->a_rel_level, 0, nsmps);
    return OK;
}

int ar_curve_init(CSOUND* csound, ar_curve* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    curve_ar_init(&obj->env, 0.5, 0.0, 0.0, 0.001, 0.0, 0.0, 0.001, 0.0, sr);
    return OK;
}

int ar_curve_vector(CSOUND* csound, ar_curve* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    curve_ar_tick_block(&obj->env, obj->a_out, obj->a_gate, obj->a_gate_thresh,
                        obj->a_start_level, obj->a_atk_sec, obj->a_atk_crv,
                        obj->a_atk_level, obj->a_rel_sec, obj->a_rel_crv,
                        obj->a_rel_level, 0, nsmps);
    return OK;
}

int adsr_curve_init(CSOUND* csound, adsr_curve* obj) {
    (void) csound;
    MYFLT sr = GetLocalSr(&obj->h);

    curve_adsr_init(&obj->env, 0.5, 0.0, 0.0, 0.001, 0.0, 0.0, 0.001, 0.0, 0.0, 0.001,
                    0.0, sr);
    return OK;
}

int adsr_curve_vector(CSOUND* csound, adsr_curve* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    curve_adsr_tick_block(&obj->env, obj->a_out, obj->a_gate, obj->a_gate_thresh,
                          obj->a_start_level, obj->a_atk_sec, obj->a_atk_level,
                          obj->a_atk_crv, obj->a_dcy_sec, obj->a_dcy_crv,
                          obj->a_sustain_level, obj->a_rel_sec, obj->a_rel_crv,
                          obj->a_rel_level, 0, nsmps);
    return OK;
}
