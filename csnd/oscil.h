#ifndef DSP_CSND_OSCIL_H
#define DSP_CSND_OSCIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>

#include <dsp/bq.h>
#include <dsp/ftable/ftable.h>
#include <dsp/oscil.h>

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_freq, *i_phase, *i_fn;  // a-aii
    FUNC* ftp;                               // the ftable pointer
    AUXCH aux;                               // hold our own copy of ftable
    ftable wt;
    oscil state;
} ftoscil3;

int ftoscil3_init(CSOUND* csound, ftoscil3* obj);
int ftoscil3_vector(CSOUND* csound, ftoscil3* obj);

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_freq, *a_phase, *i_fn;  // a-aai
    FUNC* ftp;                               // the ftable pointer
    AUXCH aux;                               // hold our own copy of ftable
    ftable wt;
    oscil state;
} ftoscil3_pm;

int ftoscil3_pm_init(CSOUND* csound, ftoscil3_pm* obj);
int ftoscil3_pm_vector(CSOUND* csound, ftoscil3_pm* obj);

#define OFTOSCIL_FILTER_N 8

// extension of the above but using oversampling
// provided with i_osrate;
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_freq, *i_phase, *i_fn, *i_osfactor;  // a-aiio (optional irate)
    FUNC* ftp;                                            // the ftable pointer
    AUXCH ft;                                             // hold our own copy of ftable
    AUXCH os_out_buf;                                     // oversamp output buffer
    AUXCH os_freq_buf;                                    // oversamp freq buffer
    size_t os_buf_sz;
    bq_non_resonant bank[OFTOSCIL_FILTER_N];  // anti-aliasing bank
    float os_fc;                              // calculated decimate cutoff
    ftable wt;
    oscil state;
    // bq_non_resonant smooth;  // smooth freq signal
    float prev;
} oftoscil3;

int oftoscil3_init(CSOUND* csound, oftoscil3* obj);
int oftoscil3_vector(CSOUND* csound, oftoscil3* obj);

/**
 * @brief band limited saw impl
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_freq, *i_phase;

    // blxoscil components
    oscil left;
    oscil right;
    blxoscil saw;

} blsaw;

int blsaw_deck_init(CSOUND* csound);
int blsaw_deck_destroy(CSOUND* csound);
int blsaw_init(CSOUND* csound, blsaw* obj);
int blsaw_vector(CSOUND* csound, blsaw* obj);

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_freq, *a_pos, *i_phase;

    // xoscil components
    oscil left;
    oscil right;
    xoscil xosc;
} smorph;

int smorph_deck_init(CSOUND* csound);
int smorph_deck_destroy(CSOUND* csound);
int smorph_init(CSOUND* csound, smorph* obj);
int smorph_vector(CSOUND* csound, smorph* obj);

#ifdef __cplusplus
}
#endif

#endif
