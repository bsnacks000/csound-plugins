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



; ssampi - sample and interpolate based on Moore
; the algo can produce data over 1.0/-1.0 in some cases
instr 1
    ain rand 1
    adur init 1
    agate mpulse 1, p4
    agate_thresh = 0.5

    aout ssampi ain, agate, agate_thresh, adur, 0, 0, 0
    ; NOTE: due to the algo ssampi can jump out of range ..
    out aout * 0.5
endin


</CsInstruments>
<CsScore>
i1 0 5 0.5
</CsScore>
</CsoundSynthesizer>
