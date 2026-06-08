#include "distort.h"
#include <dsp/maths.h>

#include <dsp/chebpoly.h>
#include <dsp/tabread.h>
#include <dsp/utils.h>
#include <stdint.h>
#include "dsp/shape.h"

#define WT_BUF_SZ 1026

// TODO: add to dsp lib
// static inline void wavetable_cubic_guardpoint(float* wt, uint32_t wt_len) {
//     wt[wt_len] = wt[0];
//     wt[wt_len + 1] = wt[1];
// }
//
/**
 * @brief - draw a line between start and stop inclusive (like numpy.linspace)
    TODO: add to dsp lib
 */
static inline void linspace(float* buf, uint32_t buf_sz, float start, float stop) {
    dsp_assert(buf_sz >= 2, "buf size must be at least 2.");

    float step = (stop - start) / (float) (buf_sz - 1);
    for (uint32_t i = 0; i < buf_sz; i++) {
        buf[i] = start + i * step;
    }
}

static float chebsaw_buf[WT_BUF_SZ] = {0};
static float line[WT_BUF_SZ - 2] = {0};

int chebsaw_tab_init(CSOUND* csound) {
    (void) csound;

    const uint32_t wt_len = WT_BUF_SZ - 2;

    // create bipolar linear bipolar ramp for the shaper
    // fill pow2_sz - 2 with the line
    linspace(line, wt_len, -1.0, 1.0);

    // ~saw wave coeffs from csound gen13 example
    float h[16] = {
        0.0,  100.0, -50.0, -33.0, 25.0,  20.0, -16.7, -14.2,
        12.5, 11.1,  -10.0, -9.09, 8.333, 7.69, -7.14, -6.67,
    };

    // calculate the chebyshev waveshape and set guard point for tabread
    chebyshev_fill(chebsaw_buf, line, wt_len, h, 16);
    wavetable_cubic_guardpoint(chebsaw_buf, wt_len);

    return OK;
}

int chebsaw_init(CSOUND* csound, chebsaw* obj) {
    (void) csound;
    (void) obj;

    tabread_init(&obj->tr, chebsaw_buf, WT_BUF_SZ);

    return OK;
}

int chebsaw_vector(CSOUND* csound, chebsaw* obj) {
    (void) csound;

    float* a_out = (float*) obj->a_out;
    float* a_in = (float*) obj->a_in;

    uint32_t nsmps = GetLocalKsmps(&obj->h);

    // covert to unipolar and scale to len wavetab len
    // (-1,1) -> (0, N)
    scale_block(a_out, a_in, 0.5, 0, nsmps);
    dc_block(a_out, a_out, 0.5, 0, nsmps);
    scale_block(a_out, a_out, (float) (obj->tr.wt_len_ - 1), 0, nsmps);

    // read off the waveshaped value
    tabread3_tick_block(&obj->tr, a_out, a_out, 0, nsmps);

    return OK;
}

typedef enum {
    HARD_CLIP = 0,
    EXP_CLIP,
    TANH_CLIP,
    ATAN_CLIP,
} saturator_type;

static int set_saturator_callback(saturator_type mode, saturator_func* cb) {
    switch (mode) {
        case HARD_CLIP: {
            *cb = hard_clip_block;
            break;
        }
        case EXP_CLIP: {
            *cb = exp_clip_block;
            break;
        }
        case TANH_CLIP: {
            *cb = fast_tanh_clip_block;
            break;
        }
        case ATAN_CLIP: {
            *cb = fast_atan_clip_block;
            break;
        }
        default:
            return 1;
    }
    return 0;
}

int saturator_init(CSOUND* csound, saturator* obj) {
    (void) csound;

    int mode = (int) *obj->i_mode;

    saturator_func cb;

    int err;
    if ((err = set_saturator_callback(mode, &cb)) != 0) {
        csound->InitError(csound, "invalid mode: %d\n", mode);
        return NOTOK;
    }

    obj->func = cb;

    return OK;
}

int saturator_vector(CSOUND* csound, saturator* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    obj->func(obj->a_out, obj->a_in, obj->a_amt, 0, nsmps);

    return OK;
}
