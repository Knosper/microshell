/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:46:39 by jjesberg          #+#    #+#             */
/*   Updated: 2022/10/25 16:43:39 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct s_data
{
	int		argc;
	char	**args;
	char	**env;
	int		cd;
	int		fd_1;
	int		fd_2[2];
}	t_data;

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (i);
	while (s[i])
		i++;
	printf("i = %i\n", (int)i);
	return (i);
}

int	init_data(t_data **data, int argc, char **args, char **env)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
		return (1);
	(*data)->argc = argc;
	(*data)->args = args;
	(*data)->env = env;
	(*data)->cd = 0;
	(*data)->fd_1 = dup(STDIN_FILENO);
	return (0);
}

/*
Setze SIMO/PIPE auf NULL bzw es ist schon NULL
kopier STDIN in fd1
close fd1
execve if success = exit point
then error message
*/
int	ft_exec(t_data **data, int i)
{
	int j;

	j = 0;
	(*data)->args[i] = NULL;
	dup2((*data)->fd_1, STDIN_FILENO);
	close((*data)->fd_1);
	execve((*data)->args[0], (*data)->args, (*data)->env);
	write(2, "cannot execute: ", 17);
	while ((*data)->args[0][j])
		j += write(2, &(*data)->args[0][j], 1);
	write(2, "\n", 1);
	return (1);
}

/*
if fork is fine = ft_exec
if ft_exec error return 1
else
close fd1
and waitpid will wait till any childprocess exits and catch status
fd1 set to STDIN
*/
int	set_end_exec(t_data **data, int i)
{
	if (fork() == 0)
	{
		if (ft_exec(data, i))
			return (1);
	}
	else
	{
		close((*data)->fd_1);
		while (waitpid(-1, NULL, WUNTRACED) != -1)
			;
		(*data)->fd_1 = dup(STDIN_FILENO);
	}
	return (0);
}

int	pipe_cmd(t_data **data, int i)
{
	pipe((*data)->fd_2);
	if (fork() == 0)
	{
		dup2((*data)->fd_2[1], STDOUT_FILENO);
		close((*data)->fd_2[0]);
		close((*data)->fd_2[1]);
		if (ft_exec(data, i))
			return (1);
	}
	else
	{
		close((*data)->fd_2[1]);
		close((*data)->fd_1);
		(*data)->fd_1 = (*data)->fd_2[0];
	}
	return (0);	
}

int	start(t_data **data)
{
	int		i;

	i = 0;
	while ((*data)->args[i] && (*data)->args[i + 1])
	{
		(*data)->args = &(*data)->args[i + 1];
		while ((*data)->args[i] != NULL && strcmp((*data)->args[i], ";") && strcmp((*data)->args[i], "|"))
			i++;
		if (!strcmp((*data)->args[0], "cd"))
		{
			if (i != 2)
				printf("error: cd: bad args\n");
			else if (chdir((*data)->args[1]))
				printf("error: cd: Datei oder Verzeichnis nicht gefunden: %s\n", (*data)->args[1]);
			(*data)->cd = 1;
		}
		else if (i != 0 && ((*data)->args[i] == NULL || !strcmp((*data)->args[i], ";")))
		{
			if (set_end_exec(data, i))
				return (1);
		}
		else if (i != 0 && !!strcmp((*data)->args[i], "|"))
		{
			if (pipe_cmd(data, i))
				return (1);
		}
	}
	close((*data)->fd_1);
	return (0);
}

int	main(int argc, char **args, char **env)
{
	t_data	*data;
	char 	*path;

	path  = NULL;
	if (init_data(&data, argc, args, env))
		return (write(2, "error: fatal\n", 13));
	if (start(&data))
		return (1);
	if (data->cd == 1)
	{
		data->cd = 0;
		path = getcwd(path, 0);
		printf("path = %s\n", path);
		free(path);
	}
	free(data);
	data = NULL;
	return (0);
}