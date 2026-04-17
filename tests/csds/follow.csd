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

instr 1 ; test envfol -

    ; consts
    kfreq = 120.0
    kres = 0.75
    acf init 220.0
    aatk init 50
    aarel init 300

    ; amp envelope params
    irise = p4
    ifall = p5
    idur = p3
    ; amp env
    aamp linseg 0, idur * irise, 1.0, idur * ifall, 0

    ; amount to give to follower
    iamt = p6

    ; modulate w/ envfol
    amod envfol aamp, aatk, aarel
    amod = amod * iamt

    kmod = amod
    printk 1, kmod

    ; saw w/ filter
    ain vco aamp, kfreq, 1, 0.5
    afilt moogladder ain, acf + amod, kres

    out afilt * 0.5
endin


</CsInstruments>
<CsScore>
f1 0 65536 10 1
i1 0 5  0.25 0.5 1500.0

</CsScore>
</CsoundSynthesizer>
