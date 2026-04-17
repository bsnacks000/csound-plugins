<CsoundSynthesizer>
<CsOptions>
    --opcode-lib="./build/csnd/libblpcsnd.so"
    --opcode-lib="./build/csnd/libblpcsnd.dylib"
    -t 60
</CsOptions>
<CsInstruments>


; NOTE due to issue with gen01 args
; we can't dynamically set this internally (gen01args not populating)
; would need to use *filesr* opcode and pass in at runtime.

sr      = 48000
ksmps   = 64
nchnls  = 1
0dbfs   = 1

gift ftgen 0, 0, 0, 1, "./tests/almostjungle1.wav", 0, 0, 1


; sampler -- really exercises tabread3 under the hood
; modulation starts at 4x down to 0 and the reverse 4x
instr 1
    aspd line 4.0, p3, -4.0
    aout sampler aspd, 0, gift
    out aout * 0.75
endin


</CsInstruments>
<CsScore>
i1 0 10;

</CsScore>
</CsoundSynthesizer>
