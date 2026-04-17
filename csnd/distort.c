#include "distort.h"
#include <dsp/maths.h>

#include <dsp/ftable/chebpoly.h>
#include <dsp/ftable/ramp.h>
#include <dsp/tabread.h>
#include <dsp/utils.h>
#include <stdint.h>
#include "dsp/shape.h"

#define ft_BUF_SZ 1026

static float chebsaw_buf[ft_BUF_SZ] = {0};

static ftable chebsaw_wt;

int chebsaw_tab_init(CSOUND* csound) {
    (void) csound;
    ftable_init(&chebsaw_wt, chebsaw_buf, ft_BUF_SZ);

    int err;

    // first create bipolar linear ramp
    ft_ramp_args ramp_args = {
        .start = -1.0,
        .stop = 1.0,
        .endpoint = true,
    };

    if ((err = ft_linspace(&chebsaw_wt, &ramp_args)) != DSP_OK) {
        return NOTOK;
    }

    // ~saw wave coeffs from csound gen13 example
    float h[16] = {
        0.0,  100.0, -50.0, -33.0, 25.0,  20.0, -16.7, -14.2,
        12.5, 11.1,  -10.0, -9.09, 8.333, 7.69, -7.14, -6.67,
    };

    ft_chebpoly_args cheby_args;
    ft_chebpoly_args_init(&cheby_args, h, 16);

    if ((err = ft_chebpoly(&chebsaw_wt, &cheby_args)) != DSP_OK) {
        return NOTOK;
    }

    return OK;
}

int chebsaw_init(CSOUND* csound, chebsaw* obj) {
    (void) csound;
    (void) obj;

    tabread_init(&obj->tr, &chebsaw_wt);

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
    scale_block(a_out, a_out, (float) (chebsaw_wt.len - 1), 0, nsmps);

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
