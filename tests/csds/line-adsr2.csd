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

; line-adsr2: test with vco2 gate signal
; test modulating most of the signals
instr 1
    klin line 0.25, p3, 3.5
    agate vco2 1.0, klin, 2, 0.5

    agate_thresh = 0.5
    astart_level = 0.0
    aatk_sec line 0.2, p3, 0.01
    aatk_level line 0.5, p3, 0.24
    adecay_sec line 0.16, p3, 0.1
    asustain_level line 0.25, p3, 0.35
    arel_sec line 0.3, p3, 0.1
    arel_level = 0.0

    aamp adsr_line agate, agate_thresh, astart_level, \
        aatk_sec, aatk_level, adecay_sec, asustain_level, \
        arel_sec, arel_level

    ; NOTE good lag setting for resets ... minimal noise in the jump
    aamp lag aamp, 0.05
    asig oscili aamp, 220, 1
    out asig
endin


</CsInstruments>
<CsScore>
f1 0 8192 10 1
i1 0 10

</CsScore>
</CsoundSynthesizer>
