/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 22:12:50 by jjesberg          #+#    #+#             */
/*   Updated: 2022/10/21 16:10:36 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro.h"
#include <sys/wait.h>

void	free_data(t_data **data)
{
	if (*data)
	{
		if ((*data)->cmd)
			ft_cleansplit((*data)->cmd);
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

int	error(int error, t_data **data, char *s)
{
	if (data)
		free_data((t_data **)data);
	if (error == DATA_ERROR)
		printf("data error\n");
	if (error == FREE_ERROR)
		printf("free data error\n");
	if (error == PARSE_ERROR)
		printf("parsing error\n");
	if (error == MALLOC_ERROR)
		printf("malloc fail\n");
	if (error == EXECV)
		printf("exec fail\n");
	if (error == CD_ARG_ERROR)
		printf("error: cd: bad arguments\n");
	if (error == CD_ERROR)
		printf("error: cd: %s: Datei oder Verzeichnis nicht gefunden\n", s);
	return (error);
}

int	init_data(t_data **data, int argc, char **args, char **env)
{
	*data = malloc(sizeof(t_data));
	if (cover_malloc(*data))
		return (error(MALLOC_ERROR, NULL, NULL));
	(*data)->argc = argc;
	(*data)->pipes = 0;
	(*data)->ret = 0;
	(*data)->args = args;
	(*data)->cmd = ft_splitdup((*data)->args + 1);
	(*data)->cmd_len = ft_splitlen((*data)->cmd);
	(*data)->env = env;
	(*data)->fd_1 = dup(STDIN_FILENO);
	(*data)->fd_2[0] = 0;
	(*data)->fd_2[1] = 0;
	return (0);
}

int ft_execute(t_data *data, int i)
{
	data->cmd[i] = NULL;
	dup2(data->fd_1, STDIN_FILENO);
	close(data->fd_1);
	execve(data->cmd[0], data->cmd, data->env);
	return (ft_putstr_fd2("error: cannot execute ", data->cmd[0]));
}

int	cd(t_data *data)
{
	int	ret;

	ret = -1;
	if (check_type(data->cmd[0]) == CD_CMD)
	{
		if (data->cmd_len != 2)
			return (error(CD_ARG_ERROR, NULL, NULL));
		ret = chdir(data->cmd[1]);
		if (ret != 0)
			return (error(CD_ERROR, NULL, data->cmd[1]));
		printf("CD CMD succes:\n");
		return (1);
	}
	return (ret);
}

int	exec_list(t_data *data)
{
	int	i;

	i = 0;
	if (cd(data) != -1)
		return (0);
	while (i < data->cmd_len)
	{
		if (check_type(data->cmd[i]) == SIMO || check_type(data->cmd[i]) == NILL)
		{
			if (fork() == 0)
			{
				if (ft_execute(data, i))
					return (1);
			}
			else
			{
				close(data->fd_1);
				while(waitpid(-1, NULL, WUNTRACED) != -1)
					;
				data->fd_1 = dup(STDIN_FILENO);
			}
		}
		else if (check_type(data->cmd[i]) == PIPE)
		{
			pipe(data->fd_2);
			if (fork() == 0)
			{
				dup2(data->fd_2[1], STDOUT_FILENO);
				close(data->fd_2[0]);
				close(data->fd_2[1]);
				if (ft_execute(data, i))
					return (1);
			}
			else
			{
				close(data->fd_2[1]);
				close(data->fd_1);
				data->fd_1 = data->fd_2[0];
			}
		}
		i++;
	}
	if ((check_type(data->cmd[i]) == NILL))
	{
		if (fork() == 0)
		{
			if (ft_execute(data, i))
				return (1);
		}
		else
		{
			close(data->fd_1);
			while(waitpid(-1, NULL, WUNTRACED) != -1)
				;
			data->fd_1 = dup(STDIN_FILENO);
		}
	}
	close(data->fd_1);
	return (0);
}

int main(int argc, char **args, char **env)
{
	t_data	*data;
	int		ret;
	int		fd;
	char *path;

	path = NULL;
	ret = 0;
	if (argc <= 1)
		return (0);
	if (init_data(&data, argc, args, env))
		return (error(DATA_ERROR, &data, NULL));
	if (exec_list(data))
		return (error(EXECV, &data, NULL));
	free_data(&data);
	/*path = getcwd(path, 0);
	printf("path = %s\n", path);
	free(path);*/
	return (ret);
}
