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

; curve - make 3 envelopes with 2 segments
; 1. concave 2. convex  3. linear
instr 1;
    aramp ccurve p4, p3, p5, p6
    asig oscili aramp , 220, 1

    out asig * 0.7
endin;

</CsInstruments>
<CsScore>
    f1 0 8192 10 1
    i1 0 1      0.0 1.0 8.0
    i. + 1      1.0 0.0 -8.0

    i. + 1      0.0 1.0 -8.0
    i. + 1      1.0 0.0 8.0

    i. + 1      0.0 1.0 0.01
    i. + 1      1.0 0.0 -0.01

</CsScore>
</CsoundSynthesizer>
