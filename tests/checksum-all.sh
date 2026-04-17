#!/bin/bash

# NOTE: run from root .. will render all files locally
set -e

csd_dir="./tests/regression/csds"

for csd in "$csd_dir"/*; do
    if [ -f "$csd" ]; then
        bn=$(basename "$csd")
        echo $bn
        uv run ./tests/regression/checksum.py -n "$bn"
    fi
done
