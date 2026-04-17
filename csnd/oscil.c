
#include "oscil.h"

#include <dsp/maths.h>
#include <dsp/resample.h>
#include <dsp/utils.h>
#include <stdint.h>

int ftoscil3_init(CSOUND* csound, ftoscil3* obj) {
    (void) csound;
    (void) obj;

    FUNC* ftp;
    if ((ftp = csound->FTFind(csound, obj->i_fn)) == NULL) {
        return csound->InitError(csound, Str("No table for ftoscil3.\n"));
    }
    obj->ftp = ftp;
    uint32_t pow2_len = ftp->flen;

    // check pow2_len from ftable
    if (!is_pow2(pow2_len)) {
        return csound->InitError(csound, Str("Ftable is not a pow2.\n"));
    }

    uint32_t buf_sz = pow2_len + 2;  // add 2 guard points for my cubic
    // interpolation.

    // allocate our own buffer for the ftable
    csound->AuxAlloc(csound, buf_sz * sizeof(MYFLT), &obj->aux);
    if (obj->aux.auxp == NULL) {
        return csound->InitError(csound, "AuxAlloc failed\n");
    }

    // // copy our pow2_table
    MYFLT* buf = (MYFLT*) obj->aux.auxp;
    for (size_t i = 0; i < pow2_len; i++) {
        buf[i] = ftp->ftable[i];
    }

    // set guard points
    buf[pow2_len] = buf[0];
    buf[pow2_len + 1] = buf[1];

    // handle phase
    float phase = clamp(*obj->i_phase, 0.0, 1.0);

    MYFLT sr = GetLocalSr(&obj->h);

    ftable_init(&obj->wt, buf, buf_sz);

    dsp_err err;

    if ((err = oscil_init(&obj->state, &obj->wt, 440.0, phase, sr)) != DSP_OK) {
        return csound->InitError(csound, "oscil_init: err\n");
    }
    return OK;
}

int ftoscil3_vector(CSOUND* csound, ftoscil3* obj) {
    (void) csound;
    uint32_t nsamps = GetLocalKsmps(&obj->h);

    oscil3_tick_block(&obj->state, obj->a_out, obj->a_freq, 0, nsamps);

    return OK;
}

int ftoscil3_pm_init(CSOUND* csound, ftoscil3_pm* obj) {
    (void) csound;
    (void) obj;

    FUNC* ftp;
    if ((ftp = csound->FTFind(csound, obj->i_fn)) == NULL) {
        return csound->InitError(csound, Str("No table for ftoscil3.\n"));
    }
    obj->ftp = ftp;
    uint32_t pow2_len = ftp->flen;

    // check pow2_len from ftable
    if (!is_pow2(pow2_len)) {
        return csound->InitError(csound, Str("Ftable is not a pow2.\n"));
    }

    uint32_t buf_sz = pow2_len + 2;  // add 2 guard points for my cubic
    // interpolation.

    // allocate our own buffer for the ftable
    csound->AuxAlloc(csound, buf_sz * sizeof(MYFLT), &obj->aux);
    if (obj->aux.auxp == NULL) {
        return csound->InitError(csound, "AuxAlloc failed\n");
    }

    // // copy our pow2_table
    MYFLT* buf = (MYFLT*) obj->aux.auxp;
    for (size_t i = 0; i < pow2_len; i++) {
        buf[i] = ftp->ftable[i];
    }

    // set guard points
    buf[pow2_len] = buf[0];
    buf[pow2_len + 1] = buf[1];

    MYFLT sr = GetLocalSr(&obj->h);

    ftable_init(&obj->wt, buf, buf_sz);

    dsp_err err;
    if ((err = oscil_init(&obj->state, &obj->wt, 440.0, 0.0, sr)) != DSP_OK) {
        return csound->InitError(csound, "oscil_init: err\n");
    }
    return OK;
}

int ftoscil3_pm_vector(CSOUND* csound, ftoscil3_pm* obj) {
    (void) csound;
    uint32_t nsamps = GetLocalKsmps(&obj->h);

    oscil3_pm_tick_block(&obj->state, obj->a_out, obj->a_freq, obj->a_phase, 0, nsamps);

    return OK;
}

int oftoscil3_init(CSOUND* csound, oftoscil3* obj) {

    FUNC* ftp;
    if ((ftp = csound->FTFind(csound, obj->i_fn)) == NULL) {
        return csound->InitError(csound, Str("No table for ftoscil3.\n"));
    }
    obj->ftp = ftp;

    uint32_t pow2_len = ftp->flen;
    // check pow2_len from ftable
    if (!is_pow2(pow2_len)) {
        return csound->InitError(csound, Str("Ftable is not a pow2.\n"));
    }

    uint32_t buf_sz = pow2_len + 2;  // add 2 guard points for my cubic
    // interpolation.

    // allocate our own buffer for the ftable
    csound->AuxAlloc(csound, buf_sz * sizeof(MYFLT), &obj->ft);
    if (obj->ft.auxp == NULL) {
        return csound->InitError(csound, "AuxAlloc failed: ft\n");
    }

    // // copy our pow2_table
    MYFLT* buf = (MYFLT*) obj->ft.auxp;
    for (size_t i = 0; i < pow2_len; i++) {
        buf[i] = ftp->ftable[i];
    }

    // // set guard points
    buf[pow2_len] = buf[0];
    buf[pow2_len + 1] = buf[1];

    // init ftable.
    ftable_init(&obj->wt, buf, buf_sz);

    // // handle phase
    float phase = clamp(*obj->i_phase, 0.0, 1.0);

    MYFLT sr = GetLocalSr(&obj->h);
    uint32_t ksmps = GetLocalKsmps(&obj->h);

    // validate the oversampling rate...
    // Any non-pow2 up to 16 defaults to 2x...
    uint32_t osfactor = (int) *obj->i_osfactor;
    switch (osfactor) {
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        case 16:
            break;
        default:
            csound->Message(csound, "Default oversampling rate (2x).\n");
            osfactor = 2;
    };

    // calculate the oversampling buffer size needed...
    obj->os_buf_sz = osfactor * ksmps;

    // obviously alot of allocs ... should alloc once and set pointers to correctly
    // index into the single memory block but this is less error prone to set up..
    csound->AuxAlloc(csound, obj->os_buf_sz * sizeof(MYFLT), &obj->os_out_buf);
    if (obj->os_out_buf.auxp == NULL) {
        return csound->InitError(csound, "AuxAlloc failed: os_out_buf.\n");
    }

    csound->AuxAlloc(csound, obj->os_buf_sz * sizeof(MYFLT), &obj->os_freq_buf);
    if (obj->os_freq_buf.auxp == NULL) {
        return csound->InitError(csound, "AuxAlloc failed: os_freq_buf.\n");
    }

    // set up the decimation filter bank and set the os_fc
    float osrate = sr * (float) osfactor;
    float fc = sr / 3.25;
    obj->os_fc = fc;
    obj->prev = 0.0;

    // build the cascade
    for (int i = 0; i < OFTOSCIL_FILTER_N; i++) {
        bq_non_resonant_init(&obj->bank[i], BQ_BUTTERWORTH, fc, osrate);
    }

    // bq_non_resonant_init(&obj->smooth, BQ_FIRST_ORDER_LPF, fc, osrate);

    csound->Message(csound, "Oversampling factor %d. Sr=%f\n", osfactor, osrate);
    // oscil finally at the oversampling rate

    dsp_err err;
    if ((err = oscil_init(&obj->state, &obj->wt, 440.0, phase, osrate)) != DSP_OK) {
        csound->InitError(csound, "oscil_init: err\n");
    }
    return OK;
}

int oftoscil3_vector(CSOUND* csound, oftoscil3* obj) {
    (void) csound;
    // original krate vector (64 probably)
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    MYFLT* os_out_buf = (MYFLT*) obj->os_out_buf.auxp;
    // MYFLT* os_in_buf = (MYFLT*) obj->os_in_buf.auxp;
    MYFLT* os_freq_buf = (MYFLT*) obj->os_freq_buf.auxp;
    size_t os_buf_sz = obj->os_buf_sz;

    // upsample freq
    upsample_back_fill(os_freq_buf, os_buf_sz, obj->a_freq, nsmps);

    // smooth freq
    // bq_non_resonant_tick_block(&obj->smooth, os_freq_buf, os_freq_buf, obj->os_fc *
    // 0.5,
    //                            os_buf_sz);

    // tick the os block and decimate
    oscil3_tick_block(&obj->state, os_out_buf, os_freq_buf, 0, os_buf_sz);

    // run the cascade
    for (int i = 0; i < OFTOSCIL_FILTER_N; i++) {
        bq_non_resonant_tick_block(&obj->bank[i], os_out_buf, os_out_buf, obj->os_fc, 0,
                                   os_buf_sz);
    }

    downsample_decimate(obj->a_out, nsmps, os_out_buf, os_buf_sz);

    return OK;
}
