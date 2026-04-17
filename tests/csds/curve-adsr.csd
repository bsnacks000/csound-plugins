<CsoundSynthesizer>
<CsOptions>
    --opcode-lib=./build/csnd/libblpcsnd.so
    --opcode-lib=./build/csnd/libblpcsnd.dylib
    -t 60
</CsOptions>
<CsInstruments>

sr = 48000
ksmps = 64
nchnls = 1
0dbfs = 1

; curve-adsr - using a vco2 as gate
; check envelope shape
instr 1
    agate vco2 1.0, p4, 2, 0.5

    agate_thresh = 0.5
    astart_level = 0.0
    aatk_sec = 0.2
    aatk_crv = 1.0
    aatk_level = 0.7
    adecay_sec = 0.3
    adecay_crv = -1.0
    asustain_level = 0.6
    arel_sec = 0.3
    arel_crv = -2.0
    arel_level = 0.0

    aamp adsr_curve agate, agate_thresh, astart_level, \
        aatk_sec, aatk_crv, aatk_level, adecay_sec, adecay_crv, asustain_level, \
        arel_sec, arel_crv, arel_level

    asig oscili aamp, 220, 1
    out asig
endin


</CsInstruments>
<CsScore>
    f1 0 8192 10 1
    i 1 0 3 0.25

</CsScore>
</CsoundSynthesizer>
