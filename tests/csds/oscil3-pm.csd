<CsoundSynthesizer>
<CsOptions>
    --opcode-lib=./build/csnd/libblpcsnd.so
    --opcode-lib=./build/csnd/libblpcsnd.dylib
    -t 60
</CsOptions>
<CsInstruments>

sr      = 48000
ksmps   = 64
nchnls  = 1
0dbfs   = 1

; oscil3-pm -- tests phase modulation capabilities added to oscil3

instr 1
    idur = p3
    ioscil_freq = p4
    imodf_lo = p5
    imodf_hi = p6

    afreq init ioscil_freq      ;init the carrier freq

    amod_frq line imodf_lo, idur, imodf_hi      ;mod freq line over event
    aphs_mod poscil3 1.0, amod_frq      ;phs mod signal
    aphs_mod = (aphs_mod + 1) * 0.5     ; unipolar .. technically not neccessary

    aout ftoscil3_pm afreq, aphs_mod, 1
    out aout * 0.75
endin


</CsInstruments>
<CsScore>
f1 0 8192 10 1
i1 0 10  100  1 250

</CsScore>
</CsoundSynthesizer>
