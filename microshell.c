/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 22:12:50 by jjesberg          #+#    #+#             */
/*   Updated: 2022/10/16 13:52:43 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro.h"

int	error(int error)
{
	if (error == DATA_ERROR)
		printf("data error\n");
	if (error == DATA_ERROR)
		printf("free data error\n");
	if (error == PARSE_ERROR)
		printf("parsing error\n");
	return (error);
}

int	init_data(t_data **data, int argc, char **args, char **env)
{
	*data = malloc(sizeof(t_data));
	if (!(*data))
		return (1);
	(*data)->argc = argc;
	(*data)->pipes = 0;
	(*data)->ret = 0;
	(*data)->args = args;
	(*data)->env = env;
	(*data)->token = malloc(sizeof(t_token));
	if (!(*data)->token)
		return (1);
	(*data)->token->next = NULL;
	(*data)->token->type = 0;
	return (0);
}

int	parse(t_data **data)
{
	int		i;
	t_token	*tmp;

	i = 1;
	(*data)->token->cmd = ft_strdup((*data)->args[i]);
	while (i < (*data)->argc)
	{
		tmp = ft_toknew((void *)ft_strdup((*data)->args[i++]));
		if (ft_strcmp(tmp->cmd, ";") == 0)
		{
			free(tmp->cmd);
			free(tmp);
			break ;
		}
		if (ft_strcmp(tmp->cmd, "|") == 0)
		{
			(*data)->pipes++;
			tmp->type = PIPE;
		}
		else if (ft_strcmp(tmp->cmd, "cd") == 0)
			tmp->type = CD_CMD;
		ft_tokadd_back(&(*data)->token, tmp);
	}
	print_tok_p(&(*data)->token);
	return (0);	
}

void	free_data(t_data **data)
{
	t_token	*tmp;
	int		i;

	i = 0;
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

int main(int argc, char **args, char **env)
{
	t_data	*data;
	int		ret;

	if (argc <= 1)
		return (0);
	if (init_data(&data, argc, args, env))
		return (error(DATA_ERROR));
	if (parse(&data))
		return (error(PARSE_ERROR));

	ret = data->ret;
	free_data(&data);
	return (ret);
}
