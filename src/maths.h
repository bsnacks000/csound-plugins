#ifndef DSP_CSND_MATHS_H
#define DSP_CSND_MATHS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csdl.h>

typedef struct {
    OPDS h;
    MYFLT *a_out, *a_in1, *a_in2;
} mult;

int mult_vector(CSOUND* csound, mult* p);

#ifdef __cplusplus
}
#endif

#endif
