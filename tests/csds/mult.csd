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

instr 1 ; mult
    a1 oscili 0.5, 440
    a2 oscili 0.5, 356
    a3 mult a1, a2
    out a3
endin

</CsInstruments>
<CsScore>
    i1 0 1
</CsScore>
</CsoundSynthesizer>
