/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 13:37:47 by jjesberg          #+#    #+#             */
/*   Updated: 2022/10/16 13:48:08 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

t_token	*ft_toklast(t_token *lst)
{
	int	i;

	i = 0;
	if (lst)
		while (lst->next)
			lst = lst->next;
	return (lst);
}

void	ft_tokadd_back(t_token **lst, t_token *new)
{
	if (lst == 0 || new == 0)
		return ;
	if (*lst == 0)
		*lst = new;
	else
		ft_toklast(*lst)->next = new;
}

t_token	*ft_toknew(void *cmd)
{
	t_token			*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (new = NULL);
	new->next = NULL;
	new->cmd = cmd;
	return (new);
}

void    print_tok_p(t_token **list)
{
    t_token  *tmp;

    tmp = (*list);
    while (tmp != NULL)
    {
		printf("text = %s\ntype = %i\n", tmp->cmd, tmp->type);
        if (tmp->next == NULL)
            break;
        tmp = tmp->next;
    }
    printf("----------------------------------------------------------------\n");
}
