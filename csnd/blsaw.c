#include <csound.h>
#include <dsp/ftable/deck.h>
#include <dsp/ftable/sinesum.h>
#include <dsp/utils.h>

#include "csdl.h"
#include "dsp/oscil.h"
#include "oscil.h"

// memory for 7 bands;
//
//
#define N_FRAMES 7
#define ft_BUF_SZ 8194

static const uint32_t bands[N_FRAMES] = {64, 32, 16, 8, 4, 2, 1};

static ft_sinesum_args* args[N_FRAMES] = {0};
static void sinesum_args_destroy(void) {
    for (int i = 0; i < N_FRAMES; i++)
        free(args[i]);
}

static ftable* wavtabs[N_FRAMES] = {0};
static void wavtabs_destroy(void) {
    for (int i = 0; i < N_FRAMES; i++)
        free(wavtabs[i]);
}

static ft_deck deck;

/**
 * @brief fail fast on bad malloc.
 */
static void check_malloc(void* p, const char* loc) {
    if (p == NULL) {
        fprintf(stderr, "malloc failed: %s\n", loc);
        exit(EXIT_FAILURE);
    }
}

static dsp_err fill_sinesum_args(void) {
    uint32_t amps_sz = 64;
    float amps[64] = {0};

    saw_amps(amps, amps_sz);

    dsp_err err;
    for (size_t i = 0; i < N_FRAMES; i++) {
        ft_sinesum_args* a = (ft_sinesum_args*) malloc(sizeof(ft_sinesum_args));
        check_malloc(a, "fill_sinesum_args");
        err =
            ft_sinesum_args_init(a, (const float*) &amps, amps_sz, 0.0, true, bands[i]);

        if (err != DSP_OK) {
            return err;
        }

        args[i] = a;
    }

    return err;
}

static void alloc_wavtabs(void) {

    for (size_t i = 0; i < N_FRAMES; i++) {
        ftable* wt = (ftable*) malloc(sizeof(ftable));
        check_malloc(wt, "fill_wavtabs:wt");
        memset(wt, 0, sizeof(ftable));

        float* buf = (float*) malloc(sizeof(float) * ft_BUF_SZ);
        check_malloc(wt, "fill_wavtabs:buf");
        memset(buf, 0, sizeof(float) * ft_BUF_SZ);

        ftable_init(wt, buf, ft_BUF_SZ);
        wavtabs[i] = wt;
    }
}

static dsp_err generate_deck(float sr) {

    dsp_err err;
    if ((err = sinesum_deck_generate(wavtabs, args, N_FRAMES, (float) sr)) != DSP_OK) {
        return err;
    }
    return err;
}

#include <stdio.h>

int blsaw_deck_init(CSOUND* csound) {

    dsp_err err;
    if ((err = fill_sinesum_args()) != DSP_OK) {
        return csound->InitError(csound, "fill_sinesum_args failed: %d\n", err);
    }
    alloc_wavtabs();

    int sr = csoundGetSr(csound);
    if ((err = generate_deck((float) sr)) != DSP_OK) {
        return csound->InitError(csound, "generate_deck failed: %d\n", err);
    }

    ft_deck_init(&deck, wavtabs, N_FRAMES);
    return OK;
}

int blsaw_deck_destroy(CSOUND* csound) {
    (void) csound;

    sinesum_args_destroy();
    wavtabs_destroy();

    return OK;
}

int blsaw_init(CSOUND* csound, blsaw* obj) {
    (void) csound;
    (void) obj;
    MYFLT sr = GetLocalSr(&obj->h);

    float phase = clamp(*obj->i_phase, 0.0, 1.0);

    dsp_err err;
    if ((err = oscil_init(&obj->left, deck.frames[0], 440.0, phase, sr)) != DSP_OK) {
        return csound->InitError(csound, "oscil_init:left %d\n", err);
    }

    if ((err = oscil_init(&obj->right, deck.frames[0], 440.0, phase, sr)) != DSP_OK) {
        return csound->InitError(csound, "oscil_init:right %d\n", err);
    }

    if ((err = blxoscil_init(&obj->saw, &deck, &obj->left, &obj->right, 440.0,
                             phase)) != DSP_OK) {
        return csound->InitError(csound, "blxoscil_init: %d\n", err);
    }

    return OK;
}

int blsaw_vector(CSOUND* csound, blsaw* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    blxoscil3_tick_block(&obj->saw, obj->a_out, obj->a_freq, 0, nsmps);
    return OK;
}
