#include "shell.h"

/**
 * main- Driver
 * @ac: argument count
 * @av: argument vector
 * Return: 0 on success
 */
int main(int ac __attribute__((unused)), char **av)
{
	char *command;

	while (1)
	{
		command = read_line();
		if (command == NULL)
			break;
		if (strlen(command) == 0)
		{
			free(command);
			continue;
		}
		exec_commands(command, av[0]);
		free(command);
	}
	if (isatty(STDIN_FILENO))
		printf("\n");
	return (0);
}

/**
 * print_env- Prints environment variables
 */
void print_env(void)
{
	char **env = environ;

	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}
