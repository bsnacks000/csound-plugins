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

; line-adsr: test with vco2 gate signal
; check env shape and timing
instr 1
    agate vco2 1.0, p4, 2, 0.5

    agate_thresh = 0.5
    astart_level = 0.0
    aatk_sec = 0.01
    aatk_level = 0.9
    adecay_sec = 0.2
    asustain_level = 0.6
    arel_sec = 0.3
    arel_level = 0.0

    aamp adsr_line agate, agate_thresh, astart_level, \
        aatk_sec, aatk_level, adecay_sec, asustain_level, \
        arel_sec, arel_level

    asig oscili aamp, 220, 1
    out asig
endin



</CsInstruments>
<CsScore>
f1 0 8192 10 1
i1 0 3 0.25

</CsScore>
</CsoundSynthesizer>
