#include "sampler.h"
#include <math.h>
#include <sys/types.h>
#include "dsp/phasor.h"
#include "dsp/tabread.h"

int sampler_init(CSOUND* csound, sampler* obj) {
    (void) csound;
    (void) obj;

    // check function table exists
    FUNC* ftp;
    if ((ftp = csound->FTFind(csound, obj->i_fn)) == NULL) {
        return csound->InitError(csound, "No table for sampler.\n");
    }
    obj->ftp = ftp;

    if (obj->ftp->nchanls != 1) {
        return csound->InitError(csound, "sampler: mono only.\n");
    }

    // try to determine the sample rate
    // making some assumptions here ... not alot of docs on how to safely do this.
    MYFLT sr;
    if (obj->ftp->gen01args.sample_rate <= 0) {
        sr = obj->ftp->gen01args.sample_rate;
    } else {
        sr = GetLocalSr(&obj->h);  // could fail fast here not gen01?
    }

    // aux mem alloc
    //
    uint32_t ksmps = GetLocalKsmps(&obj->h);
    csound->AuxAlloc(csound, ksmps * sizeof(MYFLT), &obj->freq);
    if (obj->freq.auxp == NULL) {
        return csound->InitError(csound, "AuxAlloc failed: sampler:freq\n");
    }

    csound->AuxAlloc(csound, ksmps * sizeof(MYFLT), &obj->idxs);
    if (obj->freq.auxp == NULL) {
        return csound->InitError(csound, "AuxAlloc failed: sampler:idxs\n");
    }

    // calculate duration of sample
    // MYFLT
    csound->Message(csound, "sr: %f\n", sr);
    obj->dur = ftp->flen / sr;

    // init wt with ftp
    // NOTE: flen gives the full len - guard point
    // wt will wrap the last 2 samples in this setup regardless so
    // we don't have memcopy the whole sample.
    ftable_init(&obj->wt, ftp->ftable, ftp->flen);

    // init the tabread
    tabread_init(&obj->tab, &obj->wt);

    // init the phasor
    phasor_init(&obj->ph, 1.0, *obj->i_skip, sr);

    return OK;
}

int sampler_vector(CSOUND* csound, sampler* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    MYFLT* freq = (MYFLT*) obj->freq.auxp;
    MYFLT* ticks = (MYFLT*) obj->idxs.auxp;

    // we need to cook the speed data .. converting to freq
    for (u_int32_t i = 0; i < nsmps; i++) {
        freq[i] = fabs(obj->a_speed[i]) / obj->dur;
    }

    // collect phasor ticks
    phasor_tick_block(&obj->ph, ticks, freq, 0, nsmps);

    // scale the ticks to idxs
    for (u_int32_t i = 0; i < nsmps; i++) {
        if (obj->a_speed[i] < 0.0) {  // reverse dir if negative
            ticks[i] = 1.0 - ticks[i];
        }
        ticks[i] *= obj->tab.wt->len;
    }

    // tick the tabreader
    tabreadi_tick_block(&obj->tab, obj->a_out, ticks, 0, nsmps);

    return OK;
}
