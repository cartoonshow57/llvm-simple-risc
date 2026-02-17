#!/usr/bin/env bash
set -euo pipefail

# Build a CGP1 flat binary (.bin) from a C/assembly/object input.
# Usage:
#   scripts/cgp1-make-bin.sh input.c [-o output.bin]
#   scripts/cgp1-make-bin.sh input.s [-o output.bin]
#   scripts/cgp1-make-bin.sh input.o [-o output.bin]
#
# Assumptions:
# - CGP1 toolchain is built at ./build
# - Target triple: cgp1-unknown-unknown
# - Output: flat binary (no headers) loaded at address 0x0

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
CLANG="${ROOT_DIR}/build/bin/clang"
OBJCOPY="${ROOT_DIR}/build/bin/llvm-objcopy"

if [[ ! -x "${CLANG}" ]] || [[ ! -x "${OBJCOPY}" ]]; then
  echo "error: clang/llvm-objcopy not found. Build toolchain under llvm-project/build first." >&2
  exit 1
fi

if [[ $# -lt 1 ]]; then
  echo "usage: $0 <input.{c,s,o}> [-o output.bin]" >&2
  exit 1
fi

INPUT=""
OUTPUT=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    -o)
      shift
      OUTPUT="$1"
      ;;
    *)
      if [[ -z "${INPUT}" ]]; then
        INPUT="$1"
      else
        echo "error: unexpected argument '$1'" >&2
        exit 1
      fi
      ;;
  esac
  shift
done

if [[ ! -f "${INPUT}" ]]; then
  echo "error: input file '${INPUT}' not found" >&2
  exit 1
fi

case "${INPUT}" in
  *.c) BASE="${INPUT%.c}" ;;
  *.s) BASE="${INPUT%.s}" ;;
  *.o) BASE="${INPUT%.o}" ;;
  *)   echo "error: unsupported input extension (use .c, .s, or .o)" >&2; exit 1 ;;
esac

if [[ -z "${OUTPUT}" ]]; then
  OUTPUT="${BASE}.bin"
fi

TMP_O="$(mktemp "${BASE##*/}.XXXXXX.o")"
cleanup() { rm -f "${TMP_O}"; }
trap cleanup EXIT

# Step 1: ensure we have an object file
case "${INPUT}" in
  *.o)
    cp "${INPUT}" "${TMP_O}"
    ;;
  *.c)
    "${CLANG}" --target=cgp1-unknown-unknown -c "${INPUT}" -o "${TMP_O}"
    ;;
  *.s)
    "${CLANG}" --target=cgp1-unknown-unknown -c "${INPUT}" -o "${TMP_O}"
    ;;
esac

# Step 2: convert object to flat binary
"${OBJCOPY}" -O binary "${TMP_O}" "${OUTPUT}"

echo "Wrote ${OUTPUT}"
