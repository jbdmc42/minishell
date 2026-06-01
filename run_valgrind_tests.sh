#!/usr/bin/env zsh
# run_valgrind_tests.sh - run a set of valgrind checks against the minishell binary
# Creates logs in ./valgrind_reports/ and exits with non-zero code if any test shows
# "definitely lost" or "indirectly lost" bytes.

set -euo pipefail
MINISHELL=./minishell
BUILD_CMD="make"
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"

mkdir -p valgrind_reports

echo "Building project..."
$BUILD_CMD

run_test() {
  local name="$1"; shift
  local cmds="$*"
  local out="valgrind_reports/${name}.log"

  echo "Running test: $name -> $out"
  # Feed commands to the shell and capture valgrind output
  printf '%b' "$cmds" | $VG --log-file="$out" $MINISHELL >/dev/null 2>&1 || true

  if grep -q "definitely lost: 0 bytes in 0 blocks" "$out" && \
     grep -q "indirectly lost: 0 bytes in 0 blocks" "$out"; then
    echo "PASS: $name"
    return 0
  else
    echo "FAIL: $name - see $out"
    grep -E "definitely lost|indirectly lost|still reachable" "$out" || true
    return 1
  fi
}

# testname:commands (commands use \n escapes interpreted by printf %b)
tests=(
  "simple_echo:echo test\nexit\n"
  "pipe_one:echo test | cat -e\nexit\n"
  "pipe_two:echo test | cat -e | pwd\nexit\n"
  "env_export_unset:export A=1\nenv\nunset A\nexit\n"
  "cd_pwd:cd ..\npwd\nexit\n"
)

failures=0
for t in "${tests[@]}"; do
  name="${t%%:*}"
  cmds="${t#*:}"
  if ! run_test "$name" "$cmds"; then
    failures=$((failures+1))
  fi
done

echo "Valgrind reports are in ./valgrind_reports/"
if [ $failures -ne 0 ]; then
  echo "$failures test(s) failed - investigate the corresponding logs."
  exit 2
fi

echo "All tests passed (no 'definitely lost' or 'indirectly lost' user-code blocks detected)."
exit 0
