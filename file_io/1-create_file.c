#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "main.h"

/**
 * create_file - Creates a file and writes text_content into it.
 * @filename: Name of the file to create.
 * @text_content: NULL-terminated string to write to the file.
 *
 * Return: 1 on success, -1 on failure.
 */
int create_file(const char *filename, char *text_content)
{
	int fd, bytes_written, len = 0;

	if (filename == NULL)
		return (-1);

	/* Calculate string length if text_content is provided */
	if (text_content != NULL)
	{
		while (text_content[len] != '\0')
			len++;
	}

	/*
	 * O_CREAT: Create file if it doesn't exist
	 * O_WRONLY: Open for writing only
	 * O_TRUNC: Truncate file if it already exists
	 * 0600: Permissions rw------- (read/write for owner only)
	 */
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (-1);

	if (text_content != NULL)
	{
		bytes_written = write(fd, text_content, len);
		if (bytes_written != len)
		{
			close(fd);
			return (-1);
		}
	}

	close(fd);
	return (1);
}
