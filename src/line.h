#ifndef DSP_CSND_LINE_H
#define DSP_CSND_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>
#include <dsp/line.h>

typedef struct {
    OPDS h;
    MYFLT *a_out, *i_start, *i_dur, *i_end;
    line l;
} lline;

int lline_init(CSOUND* csound, lline* obj);

int lline_vector(CSOUND* csound, lline* obj);

typedef struct {
    OPDS h;
    // a-aaaaiii
    MYFLT *a_out, *a_in, *a_gate, *a_gate_thresh, *a_dur, *i_start, *i_dur, *i_end;
    sampi s;
} ssampi;

int ssampi_init(CSOUND* csound, ssampi* obj);

int ssampi_vector(CSOUND* csound, ssampi* obj);

#ifdef __cplusplus
}
#endif

#endif
