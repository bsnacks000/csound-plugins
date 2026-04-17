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

; ar-line and ar-curve -- static and modulating tests

instr 1
    agate mpulse p4, 1

    agate_thresh = 0.5
    astart_level = 0.0
    aatk_sec = 0.01
    aatk_level = 0.75
    arel_sec = 0.1
    arel_level = 0.0

    aamp ar_line agate, agate_thresh, astart_level, aatk_sec, \
        aatk_level, arel_sec, arel_level
    asig oscili aamp, 440, 1
    out asig
endin


instr 2 ; test modulating the env signals
    klin line 0.25, p3, 6.5
    agate vco2 1.0, klin, 2, 0.5

    agate_thresh = 0.5
    astart_level = 0.0
    aatk_sec line 0.01, p3, 0.2
    aatk_level line 0.9, p3, 0.6
    arel_sec line 0.25, p3, 0.5
    arel_level = 0.0

    aamp ar_line agate, agate_thresh, astart_level, aatk_sec, \
        aatk_level, arel_sec, arel_level


    ; NOTE good lag setting for resets ... minimal noise in the jump
    ; NOTE this is csound's lag not my lag!
    aamp lag aamp, 0.05

    asig oscili aamp, 220, 1
    out asig
endin



instr 3
    agate mpulse p4, 1

    agate_thresh = 0.5
    astart_level = 0.0
    aatk_sec = 0.5
    aatk_crv = 3.0
    aatk_level = 0.75
    arel_sec = 0.5
    arel_crv = 1.0
    arel_level = 0.0

    aamp ar_curve agate, agate_thresh, astart_level, aatk_sec, \
        aatk_crv, aatk_level, arel_sec, arel_crv, arel_level
    asig oscili aamp, 220, 1
    out asig
endin


</CsInstruments>
<CsScore>
    f1 0 8192 10 1
    i1 0 3 1
    i2 3 6
    i3 9 3 1

</CsScore>
</CsoundSynthesizer>
