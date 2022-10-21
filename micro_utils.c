/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 13:37:47 by jjesberg          #+#    #+#             */
/*   Updated: 2022/10/21 16:01:22 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro.h"

int	cover_malloc(void *ptr)
{
	if (!ptr)
		return (1);
	return (0);	
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	check_type(char *s)
{
	if (!s)
		return (NILL);
	if (ft_strcmp(s, ";") == 0)
		return (SIMO);
	if (ft_strcmp(s, "|") == 0)
		return (PIPE);
	if (ft_strcmp(s, "cd") == 0)
		return (CD_CMD);
	return (EXECV);
}