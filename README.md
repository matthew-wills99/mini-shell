# mini-shell

An interactive Unix shell written in C. It covers process control, file descriptors, signals, terminal I/O and memory management.

## Features

- Runs external programs with `fork`/`execvp`, and reports exit status the way POSIX shells do (`127` for command not found, `128 + n` when a child is killed by signal `n`)
- Builtins: `cd` (including `cd -`), `pwd`, `echo` (with `-n`), `exit [n]`, `help`, `theme`
- Customizable prompt: `theme 33 '>'` sets the 256-colour code and the prompt symbol
- Ignores `SIGINT`/`SIGQUIT` in the shell and restores the defaults in child processes
- Runs non-interactively when stdin isn't a terminal: `echo pwd | ./mini-shell`

Pipes, redirection, quoting, variables and job control are in progress.

## Building

Needs a C11 compiler and GNU make.

```sh
make            # dev build:   build/dev/mini-shell
make debug      # ASan + UBSan: build/debug/mini-shell
make release    # -O2:          build/release/mini-shell
make clean
```

Every build uses `-Wall -Wextra -Werror`.

## Testing

```sh
make test                # run the test suite against the dev build
make test MODE=debug     # same tests under AddressSanitizer/UBSan
make memcheck            # same tests under Valgrind (leak check)
```

Each test is `tests/cases/NAME.in`, fed to the shell on stdin. The output is compared with `NAME.out`, plus `NAME.err` and `NAME.status` when they exist. CI runs the suite with gcc and clang in every build mode, and once under Valgrind.

## Layout

| Path | Purpose |
|---|---|
| `src/main.c` | Read-eval loop |
| `src/input.c` | Prompt and line input |
| `src/lexer.c` | Tokenizing |
| `src/exec.c` | Builtin dispatch, `fork`/`exec`/`wait` |
| `src/commands.c` | Builtin commands |
| `src/shell.c` | Shared shell state |
| `tests/` | Test runner and cases |
