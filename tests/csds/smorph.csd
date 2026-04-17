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

instr 1 ; test smorph - ramp pos 0 -> 1 -> 0 over dur N
    apos linseg 0, p3/2, 1, p3/2, 0
    afreq = 150
    aout smorph afreq, apos, 0.0
    out aout * 0.75
endin


</CsInstruments>
<CsScore>
i1 0 15;

</CsScore>
</CsoundSynthesizer>
