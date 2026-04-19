#include "shell.h"

/**
 * _getenv- custom getenv()
 * @name: name of the environment variable
 * Return: value string or NULL
 */
char *_getenv(const char *name)
{
	int i = 0;
	size_t name_len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, name_len) == 0 &&
			environ[i][name_len] == '=')
			return (environ[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

/**
 * find_in_path- search for executable file in PATH
 * @filename: file name
 * Return: full path or NULL
 */
char *find_in_path(char *filename)
{
	char *token;
	char full_path[1024];
	char *path = _getenv("PATH");
	char *path_copy;

	if (path == NULL || path[0] == '\0')
		return (NULL);

	path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);

	token = strtok(path_copy, ":");
	while (token != NULL)
	{
		sprintf(full_path, "%s/%s", token, filename);
		if (access(full_path, F_OK) == 0)
		{
			free(path_copy);
			return (strdup(full_path));
		}
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * builtin_commands- handle builtin commands
 * @args: args
 * @command: command
 */
void builtin_commands(char **args, char *command)
{
	if (strcmp(args[0], "exit") == 0)
	{
		free(command);
		exit(0);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		print_env();
	}
}

/**
 * command_path- checks relative, absolute path or searches PATH
 * @command: command
 * Return: full path or NULL
 */
char *command_path(char *command)
{
	if (command[0] != '/' && command[0] != '.')
		return (find_in_path(command));
	return (strdup(command));
}
