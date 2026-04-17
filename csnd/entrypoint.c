/**
 * @brief - the OENTRY entrypoint using csnd7 API
 *
 * See for useful decription of fields:
 * https://github.com/csound-plugins/csound-plugins
 *
 * char* opname,
 * uint16 blksz,   -- sizeof struct
 * unit16 flags,    0 for our purposes always
 * char* outtypes -- see outtype description (usually a)
 * char* intypes -- see description (usually aki)
 * int32_t  (*init)(CSOUND *, void *p);
 * int32_t  (*perf)(CSOUND *, void *p);
 * int32_t  (*deinit)(CSOUND *, void *p);  -- can register cleanup function
 * void    *useropinfo; --- user opcode parameters (not sure what this for)
 */

#include <csdl.h>

#include "blep.h"
#include "bq.h"
#include "coreDefs.h"
#include "curve.h"
#include "ddelay.h"
#include "distort.h"
#include "env.h"
#include "follow.h"
#include "lag.h"
#include "line.h"
#include "maths.h"
#include "oscil.h"
#include "phasor.h"
#include "rc.h"
#include "samph.h"
#include "sampler.h"
#include "svf.h"

// #include <stdio.h>

//  called once when the lib loads
PUBLIC int csoundModuleCreate(CSOUND* csound) {
    // printf("ModuleCreate called!\n");
    // fflush(stdout);
    int err;
    if ((err = blsaw_deck_init(csound)) != OK) {
        return csound->InitError(csound, "blsaw_deck_init: %d", err);
    }

    if ((err = smorph_deck_init(csound)) != OK) {
        return csound->InitError(csound, "smorph_deck_init: %d", err);
    }

    if ((err = chebsaw_tab_init(csound)) != OK) {
        return csound->InitError(csound, "cheby3_tab_init: %d", err);
    }

    return OK;
}

// called once before the lib is destroyed.
PUBLIC int csoundModuleDestroy(CSOUND* csound) {
    // printf("ModuleDestroy called!\n");
    blsaw_deck_destroy(csound);
    smorph_deck_destroy(csound);

    return OK;
}

static OENTRY localops[] = {
    {"mult", sizeof(mult), 0, "a", "aa", NULL, (SUBR) mult_vector, NULL, NULL, 0},
    {"ddelay2", sizeof(ddelay2), 0, "a", "aaai", (SUBR) ddelay2_init,
     (SUBR) ddelay2_vector, NULL, NULL, 0},
    {"svflp", sizeof(svflp), 0, "a", "aaaa", (SUBR) svflp_init, (SUBR) svflp_vector,
     NULL, NULL, 0},
    {"svfmm", sizeof(svfmm), 0, "aaaa", "aaaa", (SUBR) svfmm_init, (SUBR) svfmm_vector,
     NULL, NULL, 0},
    {"rclp", sizeof(rclp), 0, "a", "aa", (SUBR) rclp_init, (SUBR) rclp_vector, NULL,
     NULL, 0},
    {"rcmm", sizeof(rcmm), 0, "aaa", "aa", (SUBR) rcmm_init, (SUBR) rcmm_vector, NULL,
     NULL, 0},
    {"bqres", sizeof(bqres), 0, "a", "akki", (SUBR) bqres_init, (SUBR) bqres_vector,
     NULL, NULL, 0},
    {"bqnres", sizeof(bqnres), 0, "a", "aki", (SUBR) bqnres_init, (SUBR) bqnres_vector,
     NULL, NULL, 0},
    {"bqpara", sizeof(bqpara), 0, "a", "akkki", (SUBR) bqpara_init,
     (SUBR) bqpara_vector, NULL, NULL, 0},
    {"ftoscil3", sizeof(ftoscil3), 0, "a", "aii", (SUBR) ftoscil3_init,
     (SUBR) ftoscil3_vector, NULL, NULL, 0},

    {"ftoscil3_pm", sizeof(ftoscil3_pm), 0, "a", "aai", (SUBR) ftoscil3_pm_init,
     (SUBR) ftoscil3_pm_vector, NULL, NULL, 0},

    {"oftoscil3", sizeof(oftoscil3), 0, "a", "aiio", (SUBR) oftoscil3_init,
     (SUBR) oftoscil3_vector, NULL, NULL, 0},
    {"dphasor", sizeof(dphasor), 0, "a", "a", (SUBR) dphasor_init,
     (SUBR) dphasor_vector, NULL, NULL, 0},
    {"envfol", sizeof(envfol), 0, "a", "aaa", (SUBR) envfol_init, (SUBR) envfol_vector,
     NULL, NULL, 0},
    {"blsaw", sizeof(blsaw), 0, "a", "ai", (SUBR) blsaw_init, (SUBR) blsaw_vector, NULL,
     NULL, 0},
    {"smorph", sizeof(smorph), 0, "a", "aai", (SUBR) smorph_init, (SUBR) smorph_vector,
     NULL, NULL, 0},
    {"sampler", sizeof(sampler), 0, "a", "aii", (SUBR) sampler_init,
     (SUBR) sampler_vector, NULL, NULL, 0},
    {"lline", sizeof(lline), 0, "a", "iii", (SUBR) lline_init, (SUBR) lline_vector,
     NULL, NULL, 0},
    {"ccurve", sizeof(ccurve), 0, "a", "iiii", (SUBR) ccurve_init, (SUBR) ccurve_vector,
     NULL, NULL, 0},
    {"chebsaw", sizeof(chebsaw), 0, "a", "a", (SUBR) chebsaw_init,
     (SUBR) chebsaw_vector, NULL, NULL, 0},
    {"saturator", sizeof(saturator), 0, "a", "aai", (SUBR) saturator_init,
     (SUBR) saturator_vector, NULL, NULL, 0},
    {"blepsaw", sizeof(ssaw), 0, "a", "ai", (SUBR) ssaw_init, (SUBR) ssaw_vector, NULL,
     NULL, 0},
    {"blepsqr", sizeof(ssqr), 0, "a", "aai", (SUBR) ssqr_init, (SUBR) ssqr_vector, NULL,
     NULL, 0},
    {"ar_line", sizeof(ar_line), 0, "a", "aaaaaaa", (SUBR) ar_line_init,
     (SUBR) ar_line_vector, NULL, NULL, 0},
    {"adsr_line", sizeof(adsr_line), 0, "a", "aaaaaaaaa", (SUBR) adsr_line_init,
     (SUBR) adsr_line_vector, NULL, NULL, 0},
    {"ar_curve", sizeof(ar_curve), 0, "a", "aaaaaaaaa", (SUBR) ar_curve_init,
     (SUBR) ar_curve_vector, NULL, NULL, 0},
    {"adsr_curve", sizeof(adsr_curve), 0, "a", "aaaaaaaaaaaa", (SUBR) adsr_curve_init,
     (SUBR) adsr_curve_vector, NULL, NULL, 0},
    {"llag", sizeof(llag), 0, "a", "aa", (SUBR) llag_init, (SUBR) llag_vector, NULL,
     NULL, 0},
    {"rcladder", sizeof(rcladder), 0, "a", "aaa", (SUBR) rcladder_init,
     (SUBR) rcladder_vector, NULL, NULL, 0},
    {"ssampi", sizeof(ssampi), 0, "a", "aaaaiii", (SUBR) ssampi_init,
     (SUBR) ssampi_vector, NULL, NULL, 0},
    {"ssamph", sizeof(ssamph), 0, "a", "aaa", (SUBR) ssamph_init, (SUBR) ssamph_vector,
     NULL, NULL, 0},
    {NULL, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, 0},
};

// LINKAGE
PUBLIC int32_t csoundModuleInit(CSOUND* csound) {
    int32_t status = 0;
    for (OENTRY* oentry = &localops[0]; oentry->opname; oentry++) {
        status |= csound->AppendOpcode(csound, oentry->opname, oentry->dsblksiz,
                                       oentry->flags, oentry->outypes, oentry->intypes,
                                       oentry->init, oentry->perf, oentry->deinit);
    }
    return status;
}
