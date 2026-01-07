#!/usr/bin/env bash
set -euo pipefail

sql_file="${1}"
exe="${2}"
expect="${3}" # "ok" or "fail"

sql="$(cat "${sql_file}")"

if "${exe}" "${sql}" >/dev/null 2>&1; then
    [[ "$expect" = "ok" ]] || exit 1
else
    [[ "$expect" = "fail" ]] || exit 1
fi
