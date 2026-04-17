<CsoundSynthesizer>
<CsOptions>
    --opcode-lib="./build/csnd/libblpcsnd.so"
    --opcode-lib="./build/csnd/libblpcsnd.dylib"
    -t 60
</CsOptions>
<CsInstruments>
sr      = 48000
ksmps   = 64
nchnls  = 1
0dbfs   = 1

; basic filter sweep tests for svf, rc and biquad modules

; svf (lowpass only)
; Q [0.25, 4.0]
instr 1
    asig rand 0.3

    afreq linseg 50, p3, 10000
    aQ linseg 0.999, p3, 0.5
    adrive line 0.0, p3, 0.3

    aout svflp asig, afreq, aQ, adrive
    out aout
endin


;rc
instr 2

    asig rand 0.3

    ;afreq init 300

    afreq line 50, p3, 10000.0

    aout    rclp asig, afreq
    out aout
endin


;bqres
; Q [0.25, 10.0]
instr 3

    asig rand 0.3

    kfreq   line 50, p3, 10000
    kQ      line 0.707, p3, 3.5
    aout    bqres asig, kfreq, kQ, p4  ; p4=type (0,1,2,3,4,5)  LP, HP, BP, BS, SmAng,AP

    out aout
endin


; bqnres
instr 4

    asig rand 0.3

    kfreq   line 50, p3, 10000
    aout    bqnres asig, kfreq, p4  ; p4=type (0,1,2,3,4) = LP, HP, AP, BW, LR

    out aout
endin


; bqpara
instr 5

    asig rand 0.3

    kfreq   line 50, p3, 10000
    kQ      line 0.707, p3, 2.0
    kgain   line -10, p3, 0   ; decibels
    aout    bqpara asig, kfreq, kQ, kgain, p4  ; p4=type (0,1,2) = LShlf,Hshlf,Peak

    out aout
endin


instr 6
    asig rand 0.3

    iparam = p4

    afreq   line 50, p3, 10000
    a_lp, a_hp, a_ap    rcmm asig, afreq

    if (iparam == 0) then
        out a_lp
    elseif (iparam == 1) then
        out a_hp
    elseif (iparam == 2) then
        out a_ap
    else
        out a_lp
    endif

endin

instr 7
    asig rand 0.25

    iparam = p4

    afreq linseg 50, p3, 10000
    aQ linseg 0.999, p3, 0.5
    adrive line 0.0, p3, 0.3
    a_lp, a_hp, a_bp, a_bs    svfmm asig, afreq, aQ, adrive

    if (iparam == 0) then
        out a_lp
    elseif (iparam == 1) then
        out a_hp
    elseif (iparam == 2) then
        out a_bp
    elseif (iparam == 3) then
        out a_bs
    else
        out a_lp
    endif

endin

</CsInstruments>
<CsScore>

s
i1  + 3        ; svflp

s
i2  0 3        ; rclp

s
i3  0 3 0      ; bqres lp
i3  + . 1      ; bqres hp
i3  + . 2      ; bqres bp
i3  + . 3      ; bqres bs
i3  + . 4      ; bqres sa
i3  + . 5      ; bqres ap

s
i4  0 3 0      ; bqnres lp 1p
i4  + . 1      ; bqnres hp 1p
i4  + . 2      ; bqnres ap 1p
i4  + . 3      ; bqnres bw
i4  + . 4      ; bqnres lr

s
i5  0 3 0      ; bqpara low
i5  + . 1      ; bqpara high
i5  + . 2      ; bqpara band

s
i6  0 3 0      ; rc lp
i6  + . 1      ; rc hp
i6  + . 2      ; rc ap

s
i7  0 3 0      ; svf lp
i7  + . 1      ; svf hp
i7  + . 2      ; svf bp
i7  + . 3      ; svf bs


</CsScore>
</CsoundSynthesizer>
