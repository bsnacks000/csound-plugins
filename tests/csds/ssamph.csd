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

instr 1

    ain rand 1
    agate mpulse 1, p4
    agate_thresh = 0.5

    aout ssamph ain, agate, agate_thresh
    out (aout * 0.5) + 0.5
endin


</CsInstruments>
<CsScore>
i1 0 5 0.25
</CsScore>
</CsoundSynthesizer>
