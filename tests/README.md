# regression tests 

The regression test suite uses the csound 7 API and works with the opcodes 
generated in the `csnd` build which are mainly proof of concept instruments or 
test implementations of the core modules.

Its somewhat subjective since it relies on human analysis and introspection but 
it is also the easiest way to determine when some of these algorithms are working. 

It should be relied on as much or more then the munit test suite 
for some of the core ugen based parts of the library. 


## structure 

The folder structure and python scripts found in `regr` are essentially the test harness. 

### csds 

The csds serve as test cases. They should only test one opcode and one 
scenario at a time. Though they may contain multiple instruments. 

Csound is run in NRT mode and wav output is 32bit float

### wavs 

Referenced wav output from the runner. These can be listened back to but do not land 
in version control. 

### checksums

Once a wav is computed and approved we cache a checksum which is then version 
controlled. It can be tested against the current state of the csnd plugin implementation 
by running `test.py`. 

The test results can be analyzed to determine if lib changes effected any core output. 

## scripts (usage) 

This set of python scripts creates the test harness. 

The csound command is called as a subprocess (no API)

`render.py` 
--- 
Render a reference wav file into the `wavs` directory for inspection. 


`checksum.py`
---
Create an md5 hash from the wav file and replace in the `checksums` folder. 


`test.py`
---
The test runner recomputes checksums for each csd and compares it against its 
golden stored in checksums. If a hash has deviated significantly from what was 
originally set it is flagged and we investigate the issue.


## formalized dev workflow

The csnd test harness helps formalize the dev workflow for this lib. 

1. Add / update lib code as needed 
  * maybe add/update unittest now or later `(make coverage)` 
2. Add / update csnd7 plugin entry `(make csound)` 
3. Write csd test in `regr/csds` 
4. Render wav to `regr/wavs`
5. Analyze (-o dac or audacity, plots etc.) 
6. _Repeat (1-5) until we're happy_
7. Create checksum in `checksums` 
8. `git push`
