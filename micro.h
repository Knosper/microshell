/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 13:12:28 by jjesberg          #+#    #+#             */
/*   Updated: 2022/10/21 16:01:35 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICRO_H
# define MICRO_H

#include "Libft/libft.h"

typedef struct s_data
{
	char			**env;
	char			**args;
	char			**cmd;
	int				cmd_len;
	int				argc;
	int				fd_1;
	int				fd_2[2];
	int				ret;
	int				pipes;
}	t_data;

//Values
#define SIMO 	21
#define PIPE 	22
#define CD_CMD 	23
#define EXECV	24
#define NILL	25

//errors
# define DATA_ERROR 	10
# define MALLOC_ERROR 	11
# define PARSE_ERROR	12
# define FREE_ERROR		13
# define CD_ARG_ERROR	14
# define CD_ERROR		15

int			ft_strcmp(const char *s1, const char *s2);
int			cover_malloc(void *ptr);
int			check_type(char *s);

#endif
