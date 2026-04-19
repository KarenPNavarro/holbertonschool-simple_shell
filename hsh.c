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
 * split - splits a string into an array of tokens by spaces
 * @line: the string to split
 *
 * Return: array of strings (NULL-terminated), or NULL on failure
 */
char **split(char *line)
{
	char **args;
	char *token;
	int i;

	args = malloc(sizeof(char *) * 64);
	if (args == NULL)
		return (NULL);

	i = 0;
	token = strtok(line, " \t");
	while (token != NULL)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;
	return (args);
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
	char **args = NULL;
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

		args = split(cmd);
		if (args == NULL)
			continue;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(args);
			free(line);
			return (1);
		}

		if (pid == 0)
		{
			if (execve(args[0], args, environ) == -1)
			{
				fprintf(stderr, "%s: No such file or directory\n", args[0]);
				free(args);
				free(line);
				exit(1);
			}
		}
		else
		{
			wait(NULL);
		}
		free(args);
	}

	free(line);
	return (0);
}
