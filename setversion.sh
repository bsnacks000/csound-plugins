#!/usr/bin/env bash
set -euo pipefail

# First make sure CSND_PLUGIN_VERSION is set in CMakeLists.txt and the generated header is created.

make build

# path to generated header
VERSION_HEADER="./build/src/version.h"

# extract DSP_VERSION string using awk
VERSION=$(awk -F'"' '/#define CSND_PLUGIN_VERSION/ && $2 != "" {print $2}' "$VERSION_HEADER")

if [[ -z "$VERSION" ]]; then
    echo "Error: VERSION not found in $VERSION_HEADER"
    exit 1
fi

echo "Tag version: $VERSION"

git add -A
git commit -m "$VERSION"

# create annotated tag
git tag -a "$VERSION" -m "$VERSION"

# push tag to origin
git push origin "$VERSION"
git push
