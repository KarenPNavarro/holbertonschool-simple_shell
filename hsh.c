#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * trim - removes leading and trailing whitespace from a string
 * @s: the string to trim
 *
 * Return: pointer to the trimmed string
 */
char *trim(char *s)
{
	int i;

	while (*s == ' ' || *s == '\t')
		s++;

	i = strlen(s) - 1;
	while (i >= 0 && (s[i] == '\n' || s[i] == ' ' || s[i] == '\t'))
	{
		s[i] = '\0';
		i--;
	}
	return (s);
}

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
	char *cmd = NULL;
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

		cmd = trim(line);

		if (cmd[0] == '\0')
			continue;

		args[0] = cmd;
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
			if (execve(cmd, args, environ) == -1)
			{
				fprintf(stderr, "%s: No such file or directory\n", cmd);
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
