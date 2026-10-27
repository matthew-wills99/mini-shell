#!/bin/sh
# Usage: tests/run.sh path/to/mini-shell
#
# Each case is tests/cases/NAME.in, fed to the shell on stdin.
# Compared against NAME.out (stdout), and NAME.err (stderr) and
# NAME.status (exit code) if they exist. Missing .status means 0.
#
# Set RUNNER to wrap the shell, e.g. RUNNER="valgrind -q --error-exitcode=99"

shell=$(realpath "$1") || exit 2
root=$(cd "$(dirname "$0")" && pwd)
pass=0
fail=0

for in in "$root"/cases/*.in; do
    name=${in%.in}
    case_name=$(basename "$name")

    # Run each case in a fresh empty dir with a known HOME
    work=$(mktemp -d)
    (cd "$work" && HOME="$work" $RUNNER "$shell" < "$in" > "$work/.stdout" 2> "$work/.stderr")
    status=$?

    expected_status=0
    [ -f "$name.status" ] && expected_status=$(cat "$name.status")

    ok=1
    # Expected files may say @WORK@ where the case's work dir path goes
    sed "s|@WORK@|$work|g" "$name.out" | diff -u - "$work/.stdout" > "$work/.diff" || ok=0
    if [ -f "$name.err" ]; then
        sed "s|@WORK@|$work|g" "$name.err" | diff -u - "$work/.stderr" >> "$work/.diff" || ok=0
    fi
    [ "$status" = "$expected_status" ] || ok=0

    if [ $ok = 1 ]; then
        pass=$((pass + 1))
    else
        fail=$((fail + 1))
        echo "FAIL $case_name (status $status, expected $expected_status)"
        cat "$work/.diff"
        [ -f "$name.err" ] || cat "$work/.stderr"
    fi

    rm -rf "$work"
done

echo "$pass passed, $fail failed"
[ $fail = 0 ]
