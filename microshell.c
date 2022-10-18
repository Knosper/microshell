/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 22:12:50 by jjesberg          #+#    #+#             */
/*   Updated: 2022/10/18 06:54:17 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro.h"

void	free_data(t_data **data)
{
	t_token	*tmp;
	int		i;

	i = 0;
	if (!(*data))
		return ;
	tmp = (*data)->token;
	while (i < (*data)->argc)
	{
		i++;
		tmp = (*data)->token;
		if ((*data)->token->next)
		{
			(*data)->token = (*data)->token->next;
			free(tmp->cmd);
			free(tmp);
		}
		else
			break ;
	}
	free(tmp->cmd);
	free(tmp);
	free(*data);
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
	(*data)->env = env;
	(*data)->fd_1 = dup(STDIN_FILENO);
	(*data)->token = malloc(sizeof(t_token));
	if (cover_malloc((*data)->token))
		return (error(MALLOC_ERROR, NULL, NULL));
	(*data)->token->cmd = NULL;
	(*data)->token->join_left = 0;
	(*data)->token->join_right = 0;
	(*data)->token->next = NULL;
	(*data)->token->type = 0;
	return (0);
}

int	check_type(char *s)
{
	if (ft_strcmp(s, ";") == 0)
		return (SIMO);
	if (ft_strcmp(s, "|") == 0)
		return (PIPE);
	if (ft_strcmp(s, "cd") == 0)
		return (CD_CMD);
	return (EXECV); //todoo check path like /bin/ && echo/ls/etc
}

int	parse(t_data **data)
{
	int		i;
	t_token	*tmp;

	i = 1;
	(*data)->token->cmd = strdup((*data)->args[i++]);
	if (cover_malloc((*data)->token->cmd))
		return (error(MALLOC_ERROR, NULL, NULL));
	(*data)->token->type = check_type((*data)->token->cmd);
	while (i < (*data)->argc)
	{
		tmp = ft_toknew((void *)strdup((*data)->args[i]));
		if (cover_malloc((*data)->token->cmd))
			return (error(MALLOC_ERROR, NULL, NULL));
		tmp->type = check_type(tmp->cmd);
		ft_tokadd_back(&(*data)->token, tmp);
		i++;
	}
	print_tok_p(&(*data)->token);
	return (0);	
}

int	exec_list(t_data *data)
{
	int	cmd_len;
	int	a_len;
	int	round;
	t_token	*tmp;

	round = 0;
	tmp = data->token;
	cmd_len = ft_cmdsize(tmp);
	a_len = ft_toksize(tmp);
	while (tmp && round++ > -1)
	{
		if (tmp->type == CD_CMD && round == 1)
		{
			if (cmd_len != 2)
				return (error(CD_ARG_ERROR, NULL, NULL));
			tmp = tmp->next;
			if (chdir(tmp->cmd))
				return (error(CD_ERROR, NULL, tmp->cmd));
		}
		tmp = tmp->next;
	}
	//printf("round = %i\n", round);
	return (data->ret);
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
	if (parse(&data))
		return (error(PARSE_ERROR, &data, NULL));
	if (exec_list(data))
		return (error(EXECV, &data, NULL));
	free_data(&data);
	path = getcwd(path, 0);
	printf("path = %s\n", path);
	free(path);
	return (ret);
}
