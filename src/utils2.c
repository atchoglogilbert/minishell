/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 12:19:49 by katchogl          #+#    #+#             */
/*   Updated: 2023/01/08 14:24:12 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_isbuiltin(char *str)
{
	if (ft_strncmp (str, "echo", 5) == 0
		|| ft_strncmp (str, "cd", 3) == 0
		|| ft_strncmp (str, "pwd", 4) == 0
		|| ft_strncmp (str, "export", 7) == 0
		|| ft_strncmp (str, "unset", 6) == 0
		|| ft_strncmp (str, "env", 4) == 0
		|| ft_strncmp (str, "exit", 5) == 0)
		return (true);
	return (false);
}

void	ft_builtin(t_data *data, int i, char *builtin)
{
	if (ft_strncmp (builtin, "echo", 5) == 0)
		ft_echo (data->cmds[i].args);
	else if (ft_strncmp (builtin, "cd", 3) == 0)
	{
		if (data->cmds[i].argsc > 1)
			ft_cd (data, data->cmds[i].args[1]);
		else
			ft_cd (data, NULL); // go to user root dir (~/)
	}
	else if (ft_strncmp (builtin, "pwd", 4) == 0)
		ft_pwd (data);
	else if (ft_strncmp (builtin, "export", 7) == 0)
		ft_export (data, data->cmds[i].args);
	else if (ft_strncmp (builtin, "unset", 6) == 0)
		ft_unset (data, data->cmds[i].args);
	else if (ft_strncmp (builtin, "env", 4) == 0)
		ft_env(data);
	else if (ft_strncmp (builtin, "exit", 5) == 0)
		ft_exit (data);
}

bool	ft_isreserved(char *str)
{
	if (ft_strncmp (str, "|", 2) == 0 || ft_strncmp (str, ";", 2) == 0
		|| ft_strncmp (str, ">", 2) == 0 || ft_strncmp (str, ">>", 3) == 0
		|| ft_strncmp (str, "<", 2) == 0 || ft_strncmp (str, "<<", 3) == 0)
		return (true);
	return (false);
}