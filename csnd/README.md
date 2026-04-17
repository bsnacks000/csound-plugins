# csound plugins

These plugins are mainly used for integration testing the framework.

## inventory

At some point we can make better docs but for now here are the basics. Most are self
explanatory.

* `mult` - smoke test from their docs. Do some multiplying
* `ddelay2` - standard variable digital delay line w/ 1 tap and feedback.
* `svflp` - An SVF lowpass filter
* `svfmm` - A multimode SVF filter. Allows simultaneous LP, HP, BP, BR outs.
* `rclp` - An RC lowpass filter.
* `rcmm` - An RC multimode filter. Allows simultaneous LP, HP, AP outs.
* `bqres` - Resonant 2nd order biquad filters.
  * Selectable LP, HP, BP, BS, Reson and Allpass.
* `bqnres` - Non-Resonant 1st and 2nd order biquad filters.
  * Selectable (1st order) LP, HP, AP (2nd order) Butterworth, Linkwitz-Riley.
* `bqpara` - Parametric 2nd order filters. Lshelf, Hshelf and Peak.
* `ftoscil3` - cubic interpolating oscil (aliasing).
  * A csound ftable of the waveform must be provided.
* `oftoscil3` - cubic interpolating oscil with oversampling up to 16x. (anti-aliasing)
  * A cousnd ftable must be provided.
