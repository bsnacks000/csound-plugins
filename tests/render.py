#!/usr/bin/env python
"""
Render a single csd file located in ./csds to the ./wavs reference
folder. These are not automatiaclly rehashed and are not (and should
never be ) saved in version control.

The wav files are for analysis and listening tests.

If a wav file sounds good and we want to print then we call
checksum and pass the same name with an `md5` extension.

"""

import argparse
import os
import pathlib
import subprocess

THIS_DIR = pathlib.Path(os.path.dirname(__file__))
WAVS_DIR = THIS_DIR / "wavs"
CSDS_DIR = THIS_DIR / "csds"


def main(name: str):
    if not name.endswith(".csd"):
        csdfile = CSDS_DIR / (name + ".csd")
    else:
        csdfile = CSDS_DIR / (name)

    if not csdfile.exists():
        raise ValueError(f"{csdfile} not in {CSDS_DIR}")

    name = name.replace(".csd", "")  # in case we're carrying file ext
    wavfile = WAVS_DIR / (name + ".wav")

    subprocess.run(
        [
            "csound",
            "-m512",
            "-d",
            "-o",
            str(wavfile),
            "--format=float",
            str(csdfile),
            "-W",
        ],
        check=True,
    )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="render a wav file from a csd in csds dir."
    )
    parser.add_argument("-n", "--name", required=True, type=str)
    args = parser.parse_args()

    main(args.name)
