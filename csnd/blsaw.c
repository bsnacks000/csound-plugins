#include <errno.h>

#include <csound.h>
// #include <dsp/ftable/deck.h>
// #include <dsp/ftable/sinesum.h>
#include <dsp/sinesum.h>
#include <dsp/utils.h>

#include "csdl.h"
#include "dsp/oscil.h"
#include "oscil.h"

#define WT_BUF_SZ 8194

static inline void* xcalloc(size_t nmemb, size_t size) {
    void* bytes;
    if (!(bytes = calloc(nmemb, size))) {
        int err = errno;
        fprintf(stderr, "Fatal. Calloc failed to alloc %zu bytes. %s\n", size,
                strerror(err));
        exit(EXIT_FAILURE);
    }
    return bytes;
}

static inline void safe_free(void* data) {
    free(data);
    data = NULL;
}

static inline void wavetable_cubic_guardpoint(float* wt, uint32_t wt_len) {
    wt[wt_len] = wt[0];
    wt[wt_len + 1] = wt[1];
}

#define NHARMS_SZ 7
#define AMPS_SZ 64
static const uint32_t nharms[NHARMS_SZ] = {AMPS_SZ, 32, 16, 8, 4, 2, 1};

// static helper type to build the band limited deck components
typedef struct {
    matrix* frames;
    float* bands;
} band_limited_deck;

static band_limited_deck deck;

/**
 * @brief initialize the wt deck.
 * - allocates frames/bands
 * - because we need the SR to calculate the fundamentals we must call in blsaw init
 */
static void deck_init(CSOUND* csound) {
    float sr = csoundGetSr(csound);  // need the sr to calc the harmonics
    float* bands = xcalloc(NHARMS_SZ, sizeof(float));

    float* amps = xcalloc(AMPS_SZ, sizeof(float));
    saw_amps(amps, AMPS_SZ);

    uint32_t wt_buf_sz = WT_BUF_SZ;   // pow2 + 2
    uint32_t wt_len = WT_BUF_SZ - 2;  // pow2

    matrix* d = xcalloc(1, sizeof(matrix));  // returning ..
    float* d_buf = xcalloc(7 * wt_buf_sz, sizeof(float));

    matrix_init(d, d_buf, 7, wt_buf_sz);  // c.T  <-- target

    float* row_ = xcalloc(wt_buf_sz, sizeof(float));
    for (uint32_t i = 0; i < 7; i++) {
        // reduce the harmonic count as we go
        sinesum(row_, wt_len, amps, nharms[i], 0.0, true);
        wavetable_cubic_guardpoint(row_, wt_len);
        matrix_set_row(d, i, row_, wt_buf_sz);
        // scale to 0.7 to prevent aliasing
        bands[i] = max_fundamental(nharms[i], sr, 0.707);
    }

    safe_free(amps);
    safe_free(row_);

    deck.bands = bands;
    deck.frames = d;
}

static void deck_deinit(void) {
    safe_free(deck.frames->data);
    safe_free(deck.bands);
}

// hooks for the csound instance .. run on engine start

int blsaw_deck_init(CSOUND* csound) {
    deck_init(csound);
    return OK;
}

int blsaw_deck_destroy(CSOUND* csound) {
    (void) csound;
    deck_deinit();
    return OK;
}

int blsaw_init(CSOUND* csound, blsaw* obj) {
    (void) csound;
    (void) obj;
    MYFLT sr = GetLocalSr(&obj->h);

    float phase = clamp(*obj->i_phase, 0.0, 1.0);

    // TODO: move this bare init to blxoscil in dsp
    oscil_init(&obj->left, matrix_get_row(deck.frames, 0), deck.frames->n_cols, 100.0f,
               phase, sr);
    oscil_init(&obj->right, matrix_get_row(deck.frames, 0), deck.frames->n_cols, 100.0f,
               phase, sr);

    blxoscil_init(&obj->saw, deck.frames, &obj->left, &obj->right, deck.bands, 100.0f,
                  phase);

    return OK;
}

int blsaw_vector(CSOUND* csound, blsaw* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);

    blxoscil3_tick_block(&obj->saw, obj->a_out, obj->a_freq, 0, nsmps);
    return OK;
}
