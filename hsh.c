#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * main - entry point for the simple shell
 * @argc: argument count (unused)
 * @argv: argument vector (unused)
 * @environ: environment variables
 *
 * Return: 0 on success
 */
int main(int argc, char **argv, char **environ)
{
	char *line = NULL;
	char *args[2];
	size_t len = 0;
	ssize_t read;
	pid_t pid;

	(void)argc;
	(void)argv;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			return (0);
		}

		line[read - 1] = '\0';

		if (line[0] == '\0')
			continue;

		args[0] = line;
		args[1] = NULL;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(line);
			return (1);
		}

		if (pid == 0)
		{
			if (execve(line, args, environ) == -1)
			{
				fprintf(stderr, "./shell: No such file or directory\n");
				free(line);
				exit(1);
			}
		}
		else
		{
			wait(NULL);
		}
	}

	free(line);
	return (0);
}
