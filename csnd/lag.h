#ifndef DSP_CSND_LAG_H
#define DSP_CSND_LAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>
#include <dsp/lag.h>

/**
 * @brief test plugin for lag. csound already has lag so this is llag
 */
typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in, *t_sec;
    lag ctrl;
} llag;

int llag_init(CSOUND* csound, llag* obj);

int llag_vector(CSOUND* csound, llag* obj);

#ifdef __cplusplus
}
#endif

#endif
