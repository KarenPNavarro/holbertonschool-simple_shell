#include <stdio.h>

/**
 * _strlen - returns the length of a string
 * @s: the string to measure
 *
 * Return: the length of the string
 */
int _strlen(char *s)
{
	int len;

	len = 0;
	while (*s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}

/**
 * _puts - prints a string followed by a newline
 * @s: the string to print
 *
 * Return: void
 */
void _puts(char *s)
{
	while (*s != '\0')
	{
		putchar(*s);
		s++;
	}
	putchar('\n');
}

/**
 * main - entry point
 *
 * Return: 0 on success
 */
int main(void)
{
	char *str = "Hello, Betty!";

	_puts(str);
	return (0);
}
