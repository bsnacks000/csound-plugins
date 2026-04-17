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


; ddelay -- test static delay line
instr 1
    aenv linseg 1, 0.05, 0
    asig oscili 0.7 * aenv, 440

    kdel init p4
    kfb  init p5

    adel = kdel         ; milliseconds
    afb  = kfb          ; 0 - 1
    idelmax = 1.0

    aout ddelay2 asig, adel, afb, idelmax
    out aout * 0.5 + asig * 0.5
endin

; ddelay -- test modulation
instr 2
    ; test input
    aenv linseg 1, 0.05, 0
    asig oscili 0.4 * aenv, 440

    adel linseg p4, p3, p5
    afb linseg p6, p3, p7
    idelmax = 1.0

    aout ddelay2 asig, adel, afb, idelmax
    out aout * 0.5 + asig * 0.5
endin

</CsInstruments>
<CsScore>

    i1 0 2  200 0.6
    i2 2 3  100 800 0.7 0.3
    i2 5 2  200 10  0.6 0.8

</CsScore>
</CsoundSynthesizer>
