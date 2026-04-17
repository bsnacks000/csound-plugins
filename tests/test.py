#!/usr/bin/env python
"""
For each csd file:
    - check an md5 file exists in checksums and read it in
        - fail if not found
    - call csound via subprocess and generate a wav (test.wav) (check failure)
    - read that wav via `checksum` module and store the hash
    - compare
        - fail fast if different
clean up (test.wav)
"""

import os
import pathlib
import subprocess

import checksum

THIS_DIR = pathlib.Path(os.path.dirname(__file__))
TMP_WAV = THIS_DIR / "test.wav"
CSDS_DIR = THIS_DIR / "csds"
SUMS_DIR = THIS_DIR / "checksums"


def extract_test_name(file: pathlib.Path) -> str:
    return file.name.split(".")[0]


def read_checksums() -> dict[str, str]:
    out = {}
    for file in SUMS_DIR.iterdir():
        if not file.name.endswith(".md5"):
            raise ValueError(f"{file}: does not have .md5 extension.")
        with open(file, "r") as f:
            chsu = f.read()
        out[extract_test_name(file)] = chsu.strip("\n")
    return out


def main():
    try:
        stds = read_checksums()

        for csd in CSDS_DIR.iterdir():
            test_name = extract_test_name(csd)

            correct_sum = stds.get(test_name)
            if not correct_sum:
                raise ValueError(f"Could not find {test_name} in {SUMS_DIR}")

            print(
                f"\033[1m checking: {test_name}...\033[0m \n========================\n"
            )

            subprocess.run(
                [
                    "csound",
                    "-m512",
                    "-d",
                    "-o",
                    str(TMP_WAV),
                    "--format=float",
                    str(csd),
                    "-W",
                ],
                check=True,
            )

            tmp_sum = checksum.checksum(TMP_WAV)

            if correct_sum != tmp_sum:
                raise ValueError(
                    f"{test_name} : checksums don't match\n {correct_sum} != {tmp_sum}"
                )
            print(f"\033[1m {test_name}... \tOK.\033[0m\n=========================\n")

    finally:
        if TMP_WAV.exists():
            os.remove(str(TMP_WAV))


if __name__ == "__main__":
    main()
