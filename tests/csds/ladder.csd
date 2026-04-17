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


; ladder - this script compares our zdf ladder with csound's written by steve yi.
; the tone and warble are identical. Ours is even a bit brighter in high resonance.
instr 1

    ifreq = 100.0
    ifreq_lo = p4
    ifreq_hi = p5
    iq_lo = p6
    iq_high = p7

    ain = vco(0.5, ifreq, 1, 0.5)

    afreq = line(p4, p3, p5)
    aq = line(p6, p3, p7)

    aout = rcladder(ain, afreq, aq)


    out aout

endin

instr 2;

    ifreq = 100.0
    ifreq_lo = p4
    ifreq_hi = p5
    iq_lo = p6
    iq_high = p7

    ain = vco(0.5, ifreq, 1, 0.5)

    afreq = line(p4, p3, p5)
    aq = line(p6, p3, p7)

    aout = zdf_ladder(ain, afreq, aq)
    out aout

endin


</CsInstruments>
<CsScore>
f1 0 8192 10 1

i1 0 5  20 5000 0.1 25.0
i2 5 5  20 5000 0.1 25.0

</CsScore>
</CsoundSynthesizer>
