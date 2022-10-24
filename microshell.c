/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 22:12:50 by jjesberg          #+#    #+#             */
/*   Updated: 2022/10/24 18:40:08 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro.h"
#include <sys/wait.h>


#ifdef TEST_SH
# define TEST		1
#else
# define TEST		0
#endif

void	free_data(t_data **data)
{
	if (*data)
	{
		free(*data);
	}
}

int	ft_putstr_fd2(char *s, char *cmd)
{
	while (*s)
		write(2, s++, 1);
	if (cmd)
		while(*cmd)
			write(2, cmd++, 1);
	write(2, "\n", 1);
	return (1);
}

int	init_data(t_data **data, int argc, char *args[], char *env[])
{
	*data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	(*data)->argc = argc;
	(*data)->args = args;
	(*data)->env = env;
	(*data)->fd_1 = dup(STDIN_FILENO);
	return (0);
}

int	ft_exec(t_data **data, int i)
{
	(*data)->args[i] = NULL;
	dup2((*data)->fd_1, STDIN_FILENO);
	close((*data)->fd_1);
	execve((*data)->args[0], (*data)->args, (*data)->env);
	return (ft_putstr_fd2("error: cannot execute ", (*data)->args[0]));
}

int	start(t_data **data)
{
	int	i;

	i = 0;
	while ((*data)->args[i] && (*data)->argc > 1)
	{
		(*data)->args = &(*data)->args[i + 1];
		i = 0;

		while ((*data)->args[i] && check_type((*data)->args[i]) != PIPE && check_type((*data)->args[i]) != SIMO)
			i++;
		if (check_type((*data)->args[0]) == CD_CMD)
		{
			if (i != 2)
				ft_putstr_fd2("error: cd: bad arguments", NULL);
			else if (chdir((*data)->args[1]) != 0)
				ft_putstr_fd2("error: cd: cannot change directory to ", (*data)->args[1]);
		}
		else if (i != 0 && ((*data)->args[i] == NULL || check_type((*data)->args[i]) == SIMO))
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
		}
		else if (i != 0 && check_type((*data)->args[i]) == PIPE)
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
		}
	}
	close((*data)->fd_1);
	return (0);
}

int	main(int argc, char *args[], char *env[])
{
	t_data	*data;

	if (init_data(&data, argc, args, env))
		return (printf("error init data\n"));
	if (start(&data))
	{
		free_data(&data);
		return (1); //printf("start error\n"));
	}
	free_data(&data);
	/*char *path;
	path = NULL;
	path = getcwd(path, 0);
	printf("path = %s\n", path);
	free(path);
	if (TEST)
		while (1)
			;*/
	return (0);
}