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

; saturate - programmed with 4 modes for different levels
; of distortion -- exercises cheby waveshaping + tabread
instr 1;
    asig oscili 1.0, 220, 1

    aamt line 0.0, p3, 2.0

    adist saturator asig, aamt, p4
    out adist * 0.25
endin;

</CsInstruments>
<CsScore>
    f1 0 8192 10 1
    i1 0  4  0
    i1 +  4  1
    i1 +  4  2
    i1 +  4  3
    e
</CsScore>
</CsoundSynthesizer>
