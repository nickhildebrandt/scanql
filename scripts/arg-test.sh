#!/usr/bin/env bash
set -euo pipefail

sql_file="${1}"
exe="${2}"
expect="${3}" # "ok" or "fail"

# Validate each non-empty line as an individual SQL statement.
while IFS= read -r sql || [[ -n "$sql" ]]; do
    # Skip blank lines and comment lines
    [[ -z "$sql" || "$sql" == --* ]] && continue

    if "${exe}" "${sql}" >/dev/null 2>&1; then
        [[ "$expect" = "ok" ]] || { echo "UNEXPECTED PASS: $sql" >&2; exit 1; }
    else
        [[ "$expect" = "fail" ]] || { echo "UNEXPECTED FAIL: $sql" >&2; exit 1; }
    fi
done < "${sql_file}"
