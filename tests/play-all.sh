
#!/bin/bash

# play back all the rendered wavs via sox

set -e

wav_dir="./tests/wavs"

for wav in "$wav_dir"/*; do
    if [ -f "$wav" ]; then
        echo "now playing: ${wav}"
        play $wav
    fi
done
