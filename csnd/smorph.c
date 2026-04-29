#include <errno.h>

#include <csound.h>

#include <dsp/interpolate.h>
#include <dsp/oscil.h>
#include <dsp/sinesum.h>
#include <dsp/utils.h>

#include "oscil.h"

#define WT_BUF_SZ 8194
#define INTERP_FRAME_SZ 256
#define AMPS_SZ 64

// TODO: add to dsp lib
static inline void wavetable_cubic_guardpoint(float* wt, uint32_t wt_len) {
    wt[wt_len] = wt[0];
    wt[wt_len + 1] = wt[1];
}

// TODO: move to common.h
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

// TODO: move to common.h
static inline void safe_free(void* data) {
    free(data);
    data = NULL;
}

typedef enum {
    SINE = 0,
    BUZZ,
    SQR,
    SAW,
    TRI,
} waveform;

float* wavetable_create(uint32_t wt_len, waveform wf, uint32_t amps_sz) {
    dsp_assert(is_pow2(wt_len), "wt_len_sz must be pow2.\n");

    float* wt = (float*) xcalloc(wt_len + 2, sizeof(float));
    float* amps = (float*) xcalloc(amps_sz, sizeof(float));

    switch (wf) {
        case SINE:
            sine_amps(amps, amps_sz);
            break;
        case BUZZ:
            buzz_amps(amps, amps_sz);
            break;
        case SQR:
            sqr_amps(amps, amps_sz);
            break;
        case SAW:
            saw_amps(amps, amps_sz);
            break;
        case TRI:
            tri_amps(amps, amps_sz);
            break;
        default:
            fprintf(stderr, "wavetable_create: unreachable.\n");
            abort();
    }

    sinesum(wt, wt_len, amps, amps_sz, 0.0, true);
    safe_free(amps);

    // guardpoint
    wavetable_cubic_guardpoint(wt, wt_len);

    return wt;
}

typedef struct {
    matrix* frames;
} morph_deck;

static morph_deck deck;

static void deck_init(void) {

    // we need to interpolate out INTERP_FRAME_SZ
    // tables based on a set of three generated tables via sinesum
    const uint32_t wt_len = WT_BUF_SZ - 2;
    const uint32_t buf_sz = WT_BUF_SZ;
    const uint32_t amps_sz = AMPS_SZ;
    const uint32_t interp_frame_sz = INTERP_FRAME_SZ;

    // create the 3 wavetables to blend for the exercise ..
    float* tri = wavetable_create(wt_len, TRI, amps_sz);
    float* saw = wavetable_create(wt_len, SAW, amps_sz);
    float* sqr = wavetable_create(wt_len, SQR, amps_sz);

    // matrix buffers
    matrix a, b, c;
    matrix* d = xcalloc(1, sizeof(matrix));  // returning ..
    float* a_buf = xcalloc(3 * buf_sz, sizeof(float));
    float* b_buf = xcalloc(3 * buf_sz, sizeof(float));
    float* c_buf = xcalloc(interp_frame_sz * buf_sz, sizeof(float));
    float* d_buf = xcalloc(interp_frame_sz * buf_sz, sizeof(float));

    matrix_init(&a, a_buf, 3, buf_sz);                // a (original)
    matrix_init(&b, b_buf, buf_sz, 3);                // a.T
    matrix_init(&c, c_buf, buf_sz, interp_frame_sz);  // c (interpolated transpose of a)
    matrix_init(d, d_buf, interp_frame_sz, buf_sz);   // c.T  <-- target

    matrix_set_row(&a, 0, tri, buf_sz);
    matrix_set_row(&a, 1, saw, buf_sz);
    matrix_set_row(&a, 2, sqr, buf_sz);

    matrix_transpose(&b, &a);

    // use table_lerp to interpolate the intermediate tables
    // this will help smooth the morphing quality
    for (size_t row = 0; row < c.n_rows; row++) {
        float* in_row_ptr = matrix_get_row(&b, row);
        float* out_row_ptr = matrix_get_row(&c, row);
        table_lerp(out_row_ptr, c.n_cols, in_row_ptr, b.n_cols);
    }

    matrix_transpose(d, &c);

    safe_free(a_buf);
    safe_free(b_buf);
    safe_free(c_buf);

    deck.frames = d;
}

static void deck_deinit(void) {
    safe_free(deck.frames);
}

int smorph_deck_init(CSOUND* csound) {
    (void) csound;
    deck_init();
    return OK;
}

int smorph_deck_destroy(CSOUND* csound) {
    (void) csound;
    deck_deinit();
    return OK;
}

int smorph_init(CSOUND* csound, smorph* obj) {
    (void) csound;
    (void) obj;
    MYFLT sr = GetLocalSr(&obj->h);

    float phase = clamp(*obj->i_phase, 0.0, 1.0);

    // TODO: move this bare init to xoscil in dsp
    oscil_init(&obj->left, matrix_get_row(deck.frames, 0), deck.frames->n_cols, 100.0f,
               phase, sr);
    oscil_init(&obj->right, matrix_get_row(deck.frames, 0), deck.frames->n_cols, 100.0f,
               phase, sr);

    xoscil_init(&obj->xosc, deck.frames, &obj->left, &obj->right, 440.0f, 0.0, phase);

    return OK;
}

int smorph_vector(CSOUND* csound, smorph* obj) {
    (void) csound;
    uint32_t nsmps = GetLocalKsmps(&obj->h);
    xoscil3_tick_block(&obj->xosc, obj->a_out, obj->a_freq, obj->a_pos, 0, nsmps);
    return OK;
}
