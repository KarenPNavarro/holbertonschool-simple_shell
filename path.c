#include "shell.h"

/**
 * _getenv- custom getenv()
 * searches the environment list to find the
 * environment variable name, and returns a pointer to the
 * corresponding value string
 * @name: name of the environment variable
 * Return: path
 */

char *_getenv(const char *name)
{
	char *path = NULL;
	int i = 0;
	size_t name_len = strlen(name);

	while (environ[i])/* loop through environment list */
	{
		if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
		{
			path = environ[i] + name_len + 1;
			break;
		}
		i++;
	}
	return (path);
}

/**
 * find_in_path-  search for executable file
 * @filename: file name
 * Return: full path
 */
char *find_in_path(char *filename)
{
	char *token;
	char full_path[1024];
	char *path = _getenv("PATH");
	char *path_copy;/*avoids direct modif env variables*/

	if (path == NULL)
	{
		fprintf(stderr, "Error: PATH environment variable not found.\n");
		return (NULL);
	}
	path_copy = strdup(path);
	if (path_copy == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed.\n");
		return (NULL);
	}
	token = strtok(path_copy, ":");
	while (token != NULL)
	{
		sprintf(full_path, "%s/%s", token, filename);
		/*
		 * access() system call with F_OK flag
		 * checks if the constructed path points to
		 * an existing file
		 */
		if (access(full_path, F_OK) == 0)/* F_OK tests whether the file exists */
		{
			free(path_copy);
			return (strdup(full_path));/* return full path */
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
		return;
	}
}

/**
 * command_path- checks relative, absolute path
 * @command: command
 * Return: char* path
 */
char *command_path(char *command)
{
	char *path = NULL;

	if (command[0] != '/' && command[0] != '.')
	{
		path = find_in_path(command);/* search in PATH */
	}
	else
	{
		path = strdup(command); /* Use the provided path */
	}
	return (path);
}
