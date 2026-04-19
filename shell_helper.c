#include "shell.h"

/**
* execute- executes command using execve
* @args: command
* @prog_name: name
**/
void execute(char **args, char *prog_name)
{
	/*execve executes the program pointed to by filename*/
	if (execve(args[0], args, environ) == -1)
	{
		/*isatty test whether a file descriptor refers to a terminal*/
		if (isatty(STDIN_FILENO))
		{
			/**
			 * perror print a descriptive error message
			 * to the standard error stream (stderr)
			 */
			perror(prog_name);
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}
}

/**
* read_line- reads user input
* Return: command
**/
char *read_line(void)
{
	char *command = NULL;
	size_t len = 0;
	ssize_t getinput;

	if (isatty(STDIN_FILENO))
	{
		printf("#cisfun$ ");/* Prompt*/
		fflush(stdout);
	}
	/**
	 * getline():
	 * &command: the address of the first character
	 * position where the input string will be stored
	 * &len: address of the variable that holds the
	 * size of the input buffer
	 * stdid: input file handle
	 */
	getinput = getline(&command, &len, stdin);
	if (getinput == -1)/*handles input error*/
	{
		if (isatty(STDIN_FILENO))
		{
			printf("\n");
		}
		free(command);
		exit(0);
	}
	command[strcspn(command, "\n")] = 0;/* removes newline */
	return (command);
}

/**
* fork_execute- fork process to execute command
* @args: command
* @prog_name: program name
**/
void fork_execute(char **args, char *prog_name)
{
	pid_t pid;
	int status;

	pid = fork();/* create new process */
	if (pid == -1)
	{
		perror("Error");
		return;
	}
	else if (pid == 0)/*child*/
	{
		if (execve(args[0], args, environ) == -1)
		{
			perror(prog_name);
			exit(EXIT_FAILURE);
		}
	}
	else /*parent*/
	{
		waitpid(pid, &status, 0);/* wait for child process to finish */
	}
}

/**
* tokenize- Tokenizes a command string into
* individual arguments using delimiters
* @command: command
* @args: array to store arguments
* @argc:argc
*/
void tokenize(char *command, char **args, int *argc)
{
	char *token;
	char *delim = "\n\t\r ";

	token = strtok(command, delim);
	while (token != NULL && *argc < 99)
	{
		if (strlen(token) > 0)
		{
			args[(*argc)++] = token;/* Store token in args */
		}
		token = strtok(NULL, delim);
	}
	args[*argc] = NULL;/* NULL terminate argument list */
}



/**
 * exec_commands- parses and executes commands
 * @command: command
 * @prog_name: name
 */

void exec_commands(char *command, char *prog_name)
{
	char *args[100];
	char *path;
	int argc = 0;

	tokenize(command, args, &argc);
	if (args[0] == NULL)
	{
		return;
	}
	builtin_commands(args, command);/* checks for built-in commands */
	path = command_path(args[0]);/* get command path */
	if (path != NULL)
	{
		args[0] = path;
		fork_execute(args, prog_name);/* fork and execute command */
		free(path);
	}
	else
	{
		fprintf(stderr, "%s: command not found\n", args[0]);
	}
}
