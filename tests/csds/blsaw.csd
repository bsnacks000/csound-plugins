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

instr 1 ; test blsaw - ramp 1.0 -> N over dur
    afreq line 1, p3, p4
    aout blsaw afreq, 0
    out aout * 0.75
endin


</CsInstruments>
<CsScore>
i1 0 3 2000;

</CsScore>
</CsoundSynthesizer>
