/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakataso <dakataso@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 14:17:48 by dakataso          #+#    #+#             */
/*   Updated: 2023/06/19 14:18:18 by dakataso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*extract_line(char **str, size_t index)
{
	char	*line;
	char	*temp;

	line = ft_substr(*str, 0, index + 1);
	temp = ft_strdup(*str + index + 1);
	free(*str);
	*str = temp;
	return (line);
}

static void	*read_line(char **str)
{
	size_t	i;
	char	*line;

	i = 0;
	while ((*str)[i] != '\n' && (*str)[i] != '\0')
		i++;
	if ((*str)[i] == '\n')
		return (extract_line(str, i));
	else if (i > 0)
	{
		line = ft_strdup(*str);
		free(*str);
		*str = NULL;
		return (line);
	}
	else
	{
		free(*str);
		*str = NULL;
		return (NULL);
	}
}

static char	*allocate_buffer(int fd)
{
	char	*buffer;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, buffer, 0) < 0)
	{
		free(buffer);
		return (NULL);
	}
	return (buffer);
}

static ssize_t	read_and_append(int fd, char **str, char *buffer)
{
	ssize_t	bytes_read;

	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(*str, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!*str)
			*str = ft_strdup(buffer);
		else
			*str = ft_strjoin(*str, buffer);
	}
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	static char	*str[FD_MAX];
	char		*buffer;
	ssize_t		bytes_read;

	buffer = allocate_buffer(fd);
	if (!buffer)
	{
		free(str[fd]);
		str[fd] = NULL;
		return (NULL);
	}
	bytes_read = read_and_append(fd, &str[fd], buffer);
	free(buffer);
	buffer = NULL;
	if (!bytes_read && !str[fd])
	{
		free(str[fd]);
		return (NULL);
	}
	return (read_line(&str[fd]));
}
