/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjesberg <jjesberg@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:35:32 by jjesberg          #+#    #+#             */
/*   Updated: 2022/11/02 16:52:06 by jjesberg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_d
{
	int		argc;
	char	**args;
	char	**env;
	int		fd_1;
	int		fd_2[2];
}	t_d;

int	write_fd(int fd, char *m, char *s)
{
	int		i;

	i = 0;
	while (m[i])
		i += write(2, &m[i], 1);
	if (s)
		i += write_fd(2, s, NULL);
	i += write(2, "\n", 1);
	return (i);
}

int	init_d(t_d **d, int argc, char **args, char **env)
{
	*d = malloc(sizeof(t_d));
	if (!*d)
		return (1);
	(*d)->argc = argc;
	(*d)->args = args;
	(*d)->env = env;
	(*d)->fd_1 = dup(STDIN_FILENO);
	(*d)->fd_2[0] = 0;
	(*d)->fd_2[1] = 0;
	return (0);
}

int	ft_exec(t_d *d, int i)
{
	d->args[i] = NULL;
	dup2(d->fd_1, STDIN_FILENO);
	close(d->fd_1);
	execve(d->args[0], d->args, d->env);
	return (write_fd(2, "cannot execute ", d->args[0]));
}

int	start(t_d *d)
{
	int	i;

	i = 0;
	while (d->args[i] && d->args[i + 1])
	{
		d->args = &d->args[i + 1];
		i = 0;
		while (d->args[i] && strcmp(d->args[i], ";") && strcmp(d->args[i], "|"))
			i++;
		if (!strcmp(d->args[0], "cd"))
		{
			if (i != 2)
				write_fd(2, "error:cd:bad args", NULL);
			else if (chdir(d->args[1]))
				write_fd(2, "cannot change dir to ", d->args[1]);
		}
		else if (i != 0 && (!d->args[i] || !strcmp(d->args[i], ";")))
		{
			if (fork() == 0)
			{
				if (ft_exec(d, i))
					return (1);
			}
			else
			{
				close(d->fd_1);
				while (waitpid(-1, NULL, WUNTRACED) != -1)
					;
				d->fd_1 = dup(STDIN_FILENO);
			}
		}
		else if (i != 0 && strcmp(d->args[i], "|"))
		{
			pipe(d->fd_2);
			if (fork() == 0)
			{
				dup2(d->fd_2[1], STDOUT_FILENO);
				close(d->fd_2[0]);
				close(d->fd_2[1]);
				if (ft_exec(d, i))
					return (1);
			}
			else
			{
				close(d->fd_1);
				close(d->fd_2[1]);
				d->fd_1 = d->fd_2[0];
			}
		}
	}
	close(d->fd_1);
	return (0);
}

int	main(int argc, char **args, char **env)
{
	t_d	*d;

	if (init_d(&d, argc, args, env))
		return (write_fd(2, "error: fatal", NULL));
	if (start(d))
		return (1);
	free(d);
	return (0);
}
