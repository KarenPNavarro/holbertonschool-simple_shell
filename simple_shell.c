#include "shell.h"

/**
 * main- Driver
 * @ac: argument count
 * @av: argument vectori
 * Return: 0 on success
 */

int main(int ac __attribute__((unused)), char **av)
{
	char *command;

	while (1)/*infinite loop*/
	{
		command = read_line();/*read_line reads user input*/
		if (command == NULL)/*cecks EOF (Ctrl+D)*/
		{
			break;
		}
		if (strlen(command) == 0)/*if command line is empty*/
		{
			free(command);
			continue;
		}
		exec_commands(command, av[0]);/* executes command(s) */
		free(command);
	}
	/*isatty test whether a file descriptor refers to a terminal*/
	if (isatty(STDIN_FILENO))
	{
		printf("\n");
	}
	return (0);
}


/**
 * print_env- Prints environment variables
 */
void print_env(void)
{
	char **env = environ; /* pointer to environment variables */

	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}
