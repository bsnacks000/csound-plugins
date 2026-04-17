#!/usr/bin/env python
"""
Produce a quantized / normalized checksum for the input wav file and write it
to ./checksums with a .md5 extension and same name.
"""

import argparse
import hashlib
import os
import pathlib
import warnings

import numpy as np
from scipy.io import wavfile

THIS_DIR = pathlib.Path(os.path.dirname(__file__))
SUMS_DIR = THIS_DIR / "checksums"
WAVS_DIR = THIS_DIR / "wavs"


def create_md5_filename_from_wav(wavfile_: pathlib.Path) -> str:
    # name should return just the last part of the path
    return wavfile_.name.replace(".wav", ".md5")


def checksum(filepath: pathlib.Path, precision: int = 6) -> str:
    warnings.filterwarnings("ignore", category=wavfile.WavFileWarning)
    sr, data = wavfile.read(filepath)

    if sr != 48000:
        raise ValueError("sr should be 48k")

    if data.dtype != np.float32:
        raise ValueError("input wav for checksum must be float 32")

    quantized = np.around(data, decimals=precision)
    hash = hashlib.md5(quantized.tobytes())
    return hash.hexdigest()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Checksum from a 32bit wav")
    parser.add_argument(
        "-n",
        "--name",
        type=str,
        required=True,
        help="Name of the wav file fixture.",
    )
    args = parser.parse_args()
    wavname = args.name

    wavname = wavname.replace(".csd", "")

    if not wavname.endswith(".wav"):
        fpath = WAVS_DIR / (wavname + ".wav")
    else:
        fpath = WAVS_DIR / wavname

    if not fpath.exists():
        raise ValueError(f"{fpath} not found in wavs dir.")

    # compute checsum
    out = checksum(fpath)

    md5_fname = create_md5_filename_from_wav(fpath)
    md5_path = SUMS_DIR / md5_fname

    with open(md5_path, "w") as f:
        f.write(out)
