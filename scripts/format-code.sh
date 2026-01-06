#!/usr/bin/env bash
set -euo pipefail

find "$(git rev-parse --show-toplevel)" \
  -type f \( -name '*.c' -o -name '*.h' \) \
  -print0 \
| xargs -0 clang-format -i

