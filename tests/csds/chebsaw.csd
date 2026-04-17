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

; chebsaw - sawtooth waveshaper
; saw wave generated using cheby polynomial ftable module
; touches tabread module as well ... see csnd/distort.h
; NOTE this is not a true saw wave .. the characteristics of this waveshape
; is similar to the GEN13 csound example by Pinkston which was used as a benchmark
instr 1;
    kenv linen 0.75, 0.1, p3, 0.1
    alin linen 0.99, 2, p3, 2
    asig oscili alin, cpspch(p4), 1

    adist chebsaw asig
    out adist * kenv
endin;

</CsInstruments>
<CsScore>
    f1 0 8192 10 1
    i1 0 2  6.0
    i1 + .  7.0
    i1 + .  8.0
</CsScore>
</CsoundSynthesizer>
