/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assert.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 13:03:48 by katchogl          #+#    #+#             */
/*   Updated: 2023/01/06 20:44:59 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_assert_not_equal(t_data *data, int self, int value, t_errno err)
{
	if (self == value)
		ft_throw (data, err, NULL, true);
}

void	ft_assert_not_null(t_data *data, void *ptr)
{
	if (ptr == NULL)
		ft_throw (data, ERR_NULL_PTR, NULL, true);
}

void	ft_assert_not_dir(t_data *data, char *pathname)
{
	int	fd;

	ft_assert_not_null (data, pathname);
	fd = open (pathname, O_DIRECTORY);
	if (fd == -1)
		ft_throw (data, ERR_EISDIR, pathname, true);
	close (fd);
}

void	ft_assert_valid_permissions(t_data *data, char *pathname, int permss)
{
	int		i;
	t_args	*args;

	ft_assert_not_null (data, pathname);
	args = ft_initargs (data, pathname);
	i = -1;
	while (++i < args->count)
	{
		if (args->path == NULL)
		{
			if (pathname[0] != '/')
				args->path = ft_strdup (args->tab[i]);
			else
				args->path = ft_strjoin ("/", args->tab[i]);
		}
		else
		{
			args->temp = ft_strjoin (args->path, "/");
			free (args->path);
			args->path = args->temp;
			args->temp = ft_strjoin (args->path,args->tab[i]);
			free (args->path);
			args->path = args->temp;
		}
		if ((permss == R_OK || (permss == W_OK && i < args->count - 1))
			&& (access (args->path, F_OK) != 0 || access (args->path, permss) != 0))
		{
			if (access (args->path, F_OK) != 0)
				args->err = ENOENT;
			printf ("hello there \n");
			ft_throw (data, args->err, pathname, true);
		}
	}
	if (args->path != NULL)
		free (args->path);
	if (args->tab != NULL)
		ft_destroy_tab (args->tab);
	free (args);
}