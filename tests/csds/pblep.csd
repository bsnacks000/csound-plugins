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


; pblep - basic sweeps to look for aliasing
; should be minimal and only at high range
; inst3 checks modulates the duty of the square wave

instr 1
    afreq line 1, p3, 1000
    aout blepsaw afreq, 0
    out aout * 0.75
endin



instr 2
    afreq line 1, p3, 1000
    aduty = 0.5
    aout blepsqr afreq, aduty, 0
    out aout * 0.75
endin



instr 3
    afreq = 440
    aduty line 0.01, p3, 0.99
    aout blepsqr afreq, aduty, 0
    out aout * 0.75
endin


</CsInstruments>
<CsScore>

i1 0 3
i2 3 3
i3 6 3

</CsScore>
</CsoundSynthesizer>
