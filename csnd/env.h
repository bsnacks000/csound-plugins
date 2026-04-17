#ifndef DSP_CSOUND_ENV_H
#define DSP_CSOUND_ENV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>
#include <dsp/curve.h>
#include <dsp/line.h>
/**
 * @brief AR envelope
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_gate, *a_gate_thresh, *a_start_level, *a_atk_sec, *a_atk_level,
        *a_rel_sec, *a_rel_level;
    line_ar env;
} ar_line;

int ar_line_init(CSOUND* csound, ar_line* obj);

int ar_line_vector(CSOUND* csound, ar_line* obj);

/**
 * @brief ADSR envelope
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_gate, *a_gate_thresh, *a_start_level, *a_atk_sec, *a_atk_level,
        *a_dcy_sec, *a_sustain_level, *a_rel_sec, *a_rel_level;
    line_adsr env;
} adsr_line;

int adsr_line_init(CSOUND* csound, adsr_line* obj);

int adsr_line_vector(CSOUND* csound, adsr_line* obj);

/**
 * @brief AR envelope
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_gate, *a_gate_thresh, *a_start_level, *a_atk_sec, *a_atk_crv,
        *a_atk_level, *a_rel_sec, *a_rel_crv, *a_rel_level;
    curve_ar env;
} ar_curve;

int ar_curve_init(CSOUND* csound, ar_curve* obj);

int ar_curve_vector(CSOUND* csound, ar_curve* obj);

/**
 * @brief ADSR envelope
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_gate, *a_gate_thresh, *a_start_level, *a_atk_sec, *a_atk_crv,
        *a_atk_level, *a_dcy_sec, *a_dcy_crv, *a_sustain_level, *a_rel_sec, *a_rel_crv,
        *a_rel_level;
    curve_adsr env;
} adsr_curve;

int adsr_curve_init(CSOUND* csound, adsr_curve* obj);

int adsr_curve_vector(CSOUND* csound, adsr_curve* obj);

#ifdef __cplusplus
}
#endif

#endif
