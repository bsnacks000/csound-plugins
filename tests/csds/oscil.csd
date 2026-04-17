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

; oscil - sweep test wrappers for oscil3 (the core lib osc)
; -- oftoscil tests oversampling functionality
; -- there is a test for neg frequency

; TODO -- wrap osciln + oscili and test here as well

instr 1 ; test ftoscil3
    afreq   line 1, p3, 18000
    aout ftoscil3 afreq, 0, p4

    out aout * 0.75
endin


instr 2 ; test oftoscil3
    afreq   line 1, p3, 12000
    aout oftoscil3 afreq, 0, p4, p5

    out aout * 0.75
endin


instr 3 ; neg freq test
    afreq line 1000, p3, -1000
    aout ftoscil3 afreq, 0, 1
    out aout * 0.75
endin

</CsInstruments>
<CsScore>
f1 0 8192 10 1; sine
f2 0 8192 10 1 0 .333 0 .2 0 .143 0 .111 0 .0909 0 .077 0 .0666 0 .0588 ; sqr
f3 0 8192 10 1 0.5 0.3333 0.25 0.2 0.1666 0.1428 0.125 0.1111 0.1; saw
f4 0 8192 10 1 0 0.1111 0 0.04 0 0.0204 0 0.0123 0; tri
f5 0 8192 10 10 0 0 0 0 1 1 1 1 1 1; buzz
;-- various waveforms from above ftables (will alias)

s
i1 0 3 1
i1 + . 2
i1 + . 3
i1 + . 4
i1 + . 5

s
i2 0 3 2 2   ; 2x oversample
i2 + . . 4   ; 4x oversample
i2 + . . 8   ; 8x oversample

s
i3 0 3      ; neg freq

</CsScore>
</CsoundSynthesizer>
