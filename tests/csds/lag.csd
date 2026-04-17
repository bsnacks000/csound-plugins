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

; need a seed for randi
seed 12345

; lag - we print a randi segment as well as our lagged version
; for comparison. This is control rate signal and only visible in analysis tools.
; The smoothing of the lag filter is very clear.
instr 1
    asig = 0

    asig randi 0.9, p4
    out asig
endin

instr 2
    asig = 0
    alag = 0.1

    asig randi 0.9, p4
    asig llag asig, alag
    out asig
endin

</CsInstruments>
<CsScore>
f1 0 8192 10 1
i1 0 1 20
i2 1 2 20
</CsScore>
</CsoundSynthesizer>
