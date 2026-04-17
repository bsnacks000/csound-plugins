
#include <csound.h>
#include <dsp/ftable/deck.h>
#include <dsp/ftable/sinesum.h>
#include <dsp/interpolate.h>
#include <dsp/oscil.h>
#include <dsp/utils.h>

#include "oscil.h"

#define ftable_BUF_SZ 8194
#define INTERP_FRAME_SZ 256
#define AMPS_SZ 64

/**
 * @brief fail fast on bad malloc.
 */
// static void check_malloc(void* p, const char* loc) {
//     if (p == NULL) {
//         fprintf(stderr, "malloc failed: %s\n", loc);
//         exit(EXIT_FAILURE);
//     }
// }

// interpolated wavtabs for the deck.
ftable* wavtabs[INTERP_FRAME_SZ] = {0};
static void wavtabs_destroy(void) {
    for (int i = 0; i < INTERP_FRAME_SZ; i++)
        free(wavtabs[i]);
}

// the main deck
static ft_deck deck;

// TODO: clean this up
// break out logic into smaller pieces
int smorph_deck_init(CSOUND* csound) {

    float sr = (float) csoundGetSr(csound);
    uint32_t amps_sz = AMPS_SZ;

    float amps_saw[AMPS_SZ] = {0};
    float amps_sqr[AMPS_SZ] = {0};
    float amps_tri[AMPS_SZ] = {0};

    // fill with amps. 0=saw, 1=sqr, 2=tri
    saw_amps(amps_saw, amps_sz);
    sqr_amps(amps_sqr, amps_sz);
    tri_amps(amps_tri, amps_sz);

    ft_sinesum_args* args[3] = {0};

    args[0] = (ft_sinesum_args*) malloc(sizeof(ft_sinesum_args));
    args[1] = (ft_sinesum_args*) malloc(sizeof(ft_sinesum_args));
    args[2] = (ft_sinesum_args*) malloc(sizeof(ft_sinesum_args));

    ft_sinesum_args_init(args[0], amps_tri, amps_sz, 0.0, true, amps_sz);
    ft_sinesum_args_init(args[1], amps_saw, amps_sz, 0.0, true, amps_sz);
    ft_sinesum_args_init(args[2], amps_sqr, amps_sz, 0.0, true, amps_sz);

    // create the initial ftable deck
    ftable* initial_wavtabs[3] = {0};
    for (int i = 0; i < 3; i++) {
        ftable* wt = (ftable*) malloc(sizeof(ftable));
        float* buf = (float*) malloc(sizeof(float) * ftable_BUF_SZ);
        memset(buf, 0, sizeof(float) * ftable_BUF_SZ);
        ftable_init(wt, buf, ftable_BUF_SZ);
        initial_wavtabs[i] = wt;
    }

    // generate the initial waveforms
    dsp_err err;
    if ((err = sinesum_deck_generate(initial_wavtabs, args, 3, sr)) != DSP_OK) {
        for (int i = 0; i < 3; i++) {
            free(initial_wavtabs[i]->buf);
            free(args[i]);
        }
        return NOTOK;
    }

    // we initialize this deck so we can use the matrix fill method
    ft_deck initial_deck;
    ft_deck_init(&initial_deck, initial_wavtabs, 3);

    // we need alot of memory to facilitate the ftable interp dance.

    // m - original matrix from wavtabs shape=(3, ftable_BUF_SZ)
    matrix m;
    float* m_data = (float*) malloc(sizeof(float) * 3 * ftable_BUF_SZ);
    memset(m_data, 0, sizeof(float) * 3 * ftable_BUF_SZ);
    matrix_init(&m, m_data, 3, ftable_BUF_SZ);

    // fill the matrix from the existing deck
    ft_deck_matrix_fill(&initial_deck, &m);

    // t - transpose matrix  shape=(ftable_BUF_SZ, 3)
    matrix t;
    float* t_data = (float*) malloc(sizeof(float) * 3 * ftable_BUF_SZ);
    memset(t_data, 0, sizeof(float) * 3 * ftable_BUF_SZ);
    matrix_init(&t, t_data, ftable_BUF_SZ, 3);

    // initial transpose
    matrix_transpose(&t, &m);

    // tx - extended transpose matrix shape=(ftable_BUF_SZ, INTERP_FRAME_SZ)
    matrix tx;
    float* tx_data = (float*) malloc(sizeof(float) * ftable_BUF_SZ * INTERP_FRAME_SZ);
    memset(tx_data, 0, sizeof(float) * ftable_BUF_SZ * INTERP_FRAME_SZ);
    matrix_init(&tx, tx_data, ftable_BUF_SZ, INTERP_FRAME_SZ);

    // iter over each row of the transpose and project cols from in to out
    for (size_t row = 0; row < tx.n_rows; row++) {
        float* in_row_ptr = matrix_get_row(&t, row);
        float* out_row_ptr = matrix_get_row(&tx, row);
        npinterp(out_row_ptr, tx.n_cols, in_row_ptr, t.n_cols);
    }

    // u - final matrix shape=(INTERP_FRAME_SZ, ftable_BUF_SZ)
    matrix u;
    float* u_data = (float*) malloc(sizeof(float) * INTERP_FRAME_SZ * ftable_BUF_SZ);
    memset(u_data, 0, sizeof(float) * ftable_BUF_SZ * INTERP_FRAME_SZ);
    matrix_init(&u, u_data, INTERP_FRAME_SZ, ftable_BUF_SZ);

    // final transpose back to the original
    matrix_transpose(&u, &tx);

    // new wavtabs - these are the bois that go to xoscil
    for (int i = 0; i < INTERP_FRAME_SZ; i++) {
        ftable* wt = (ftable*) malloc(sizeof(ftable));
        float* buf = (float*) malloc(sizeof(float) * ftable_BUF_SZ);
        memset(buf, 0, sizeof(float) * ftable_BUF_SZ);

        ftable_init(wt, buf, ftable_BUF_SZ);
        float* row_ptr = matrix_get_row(&u, i);
        ftable_write(wt, row_ptr, ftable_BUF_SZ, 0);
        wavtabs[i] = wt;
    }

    // set this on the static deck .. this is what is going into
    // xoscil instances at runtime.
    ft_deck_init(&deck, wavtabs, INTERP_FRAME_SZ);

    // free initial memory
    free(u_data);
    free(tx_data);
    free(t_data);
    free(m_data);
    for (int i = 0; i < 3; i++) {
        free(initial_wavtabs[i]->buf);
        free(args[i]);
    }

    return OK;
}
int smorph_deck_destroy(CSOUND* csound) {
    (void) csound;
    wavtabs_destroy();
    return OK;
}

int smorph_init(CSOUND* csound, smorph* obj) {
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

    if ((err = xoscil_init(&obj->xosc, &deck, &obj->left, &obj->right, 440.0, 0.0,
                           phase)) != DSP_OK) {
        return csound->InitError(csound, "blxoscil_init: %d\n", err);
    }

    return OK;
}

int smorph_vector(CSOUND* csound, smorph* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);
    xoscil3_tick_block(&obj->xosc, obj->a_out, obj->a_freq, obj->a_pos, 0, nsmps);
    return OK;
}
