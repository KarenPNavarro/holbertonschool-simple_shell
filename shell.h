#ifndef SHELL
#define SHELL

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>


extern char **environ;
void execute(char **args, char *prog_name);
char *read_line(void);
void fork_execute(char **args, char *prog_name);
void exec_commands(char *command, char *prog_name);
void print_env(void);
char *find_command(char *command);
char *_getenv(const char *name);
char *find_in_path(char *filename);
void builtin_commands(char **args, char *command);
char *command_path(char *command);
void tokenize(char *command, char **args, int *argc);

#endif
