/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 13:12:28 by jjesberg          #+#    #+#             */
/*   Updated: 2022/10/18 06:43:45 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICRO_H
# define MICRO_H

#include "Libft/libft.h"

typedef struct s_token
{
	char			*cmd;
	int				join_left;
	int				join_right;
	int				type;
	struct s_token	*next;
	//struct s_token	*prev;
}	t_token;

typedef struct s_data
{
	char			**env;
	char			**args;
	int				argc;
	int				fd_1;
	int				fd_2;
	int				ret;
	int				pipes;
	t_token			*token;
}	t_data;

//Values
#define SIMO 	21
#define PIPE 	22
#define CD_CMD 	23
#define EXECV	24

//errors
# define DATA_ERROR 	10
# define MALLOC_ERROR 	11
# define PARSE_ERROR	12
# define FREE_ERROR		13
# define CD_ARG_ERROR	14
# define CD_ERROR		15

void    	print_tok_p(t_token **list);
t_token		*ft_toknew(void *cmd);
void		ft_tokadd_back(t_token **lst, t_token *new);
t_token		*ft_toklast(t_token *lst);
int			ft_strcmp(const char *s1, const char *s2);
int			cover_malloc(void *ptr);
int			ft_toksize(t_token *lst);
int			ft_cmdsize(t_token *lst);

#endif
