#include "shell.h"

int last_status = 0;
/**
 * read_line- reads user input
 * Return: command string
 */
char *read_line(void)
{
	char *command = NULL;
	size_t len = 0;
	ssize_t getinput;

	if (isatty(STDIN_FILENO))
	{
		printf("#cisfun$ ");
		fflush(stdout);
	}
	getinput = getline(&command, &len, stdin);
	if (getinput == -1)
	{
		if (isatty(STDIN_FILENO))
			printf("\n");
		free(command);
		exit(0);
	}
	command[strcspn(command, "\n")] = 0;
	return (command);
}

/**
 * fork_execute- fork process to execute command
 * @args: command args
 * @prog_name: program name
 */
void fork_execute(char **args, char *prog_name)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("Error");
		return;
	}
	else if (pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
		{
			perror(prog_name);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status); /* save last exit code */
	}
}

/**
 * tokenize- tokenizes a command string
 * @command: command
 * @args: array to store arguments
 * @argc: argument count
 */
void tokenize(char *command, char **args, int *argc)
{
	char *token;
	char *delim = "\n\t\r ";

	token = strtok(command, delim);
	while (token != NULL && *argc < 99)
	{
		if (strlen(token) > 0)
			args[(*argc)++] = token;
		token = strtok(NULL, delim);
	}
	args[*argc] = NULL;
}

/**
 * exec_commands- parses and executes commands
 * @command: command
 * @prog_name: program name
 */
void exec_commands(char *command, char *prog_name)
{
	char *args[100];
	char *path;
	int argc = 0;
	static int line_number = 0;

	line_number++;
	tokenize(command, args, &argc);
	if (args[0] == NULL)
		return;
	builtin_commands(args, command);
	path = command_path(args[0]);
	if (path != NULL)
	{
		args[0] = path;
		fork_execute(args, prog_name);
		free(path);
	}
	else
	{
		fprintf(stderr, "%s: %d: %s: not found\n", prog_name, line_number, args[0]);
		free(command);  /* free before exit to avoid leak */
		exit(127);
	}
}
