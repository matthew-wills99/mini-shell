#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#include "exec.h"
#include "commands.h"
#include "shell.h"

static int launch(char**);

int execute(char** args) {
    if(args[0] == NULL) return last_status;

    const builtin_t* builtin = find_builtin(args[0]);
    if(builtin != NULL) return builtin->fn(args);

    return launch(args);
}

static int launch(char** args) {
    pid_t pid = fork();
    if(pid < 0) {
        perror("fork");
        return 1;
    }

    if(pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        execvp(args[0], args);
        if(errno == ENOENT) {
            fprintf(stderr, "%s: command not found\n", args[0]);
            _exit(127);
        }
        perror(args[0]);
        _exit(126);
    }

    int status;
    while(waitpid(pid, &status, 0) == -1) {
        if(errno != EINTR) {
            perror("waitpid");
            return 1;
        }
    }

    if(WIFEXITED(status)) return WEXITSTATUS(status);
    if(WIFSIGNALED(status)) return 128 + WTERMSIG(status);
    return 1;
}
