/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 17:41:10 by katchogl          #+#    #+#             */
/*   Updated: 2023/01/19 20:03:57 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_isheredoc_lim(char *heredoc_lim, char *buff)
{

	if (heredoc_lim != NULL && buff != NULL)
		if (ft_strncmp(heredoc_lim, buff, ft_strlen(heredoc_lim) + 1) == 0)
			return (true);
	return (false);
}

// static bool	ft_isheredoc_lim(char *heredoc_lim, char *buff)
// {
// 	char	*heredoc_limendl;
// 	if (heredoc_lim != NULL && buff != NULL)
// 	{
// 		heredoc_limendl = ft_strjoin (heredoc_lim, "\n");
// 		if (heredoc_limendl != NULL)
// 		{
// 			if (ft_strncmp (heredoc_limendl, buff,
// 					ft_strlen (heredoc_limendl) + 1) == 0)
// 			{
// 				free (heredoc_limendl);
// 				return (true);
// 			}
// 			free (heredoc_limendl);
// 		}
// 	}
// 	return (false);
// }

// static void	ft_read(int fd, char *heredoc_lim)
// {
// 	char	buff[BUFFER_SIZE];
// 	int		count;
// 	while (1)
// 	{
// 		ft_putstr_fd ("> ", STDERR_FILENO);
// 		count = read (STDIN_FILENO, buff, BUFFER_SIZE - 1);
// 		if (count <= 0)
// 			break ;
// 		else
// 		{
// 			buff[count] = '\0';
// 			if (!ft_isheredoc_lim (heredoc_lim, buff))
// 				write (fd, buff, count);
// 			else
// 				break ;
// 		}
// 	}
// }

static void	ft_read(t_data *data, int fd, char *heredoc_lim)
{
	char	*buff;
	char	*line;
	
	ft_assert_not_null (data, data);
	while (1)
	{
		buff = readline("> ");
		ft_putstr_fd ("test", 2);
		if (buff != NULL && !ft_isheredoc_lim(heredoc_lim, buff))
		{
			line = ft_strjoin(buff, "\n");
			if (line != NULL)
				write(fd, line, ft_strlen(line));
			free(line);
			free(buff);
		}
		else
		{
			free(buff);
			break;
		}
	}
}

// static void	ft_read(int fd, char *heredoc_lim)
// {
// 	char	*buff;
// 	while (1)
// 	{
// 		ft_putstr_fd ("> ", STDERR_FILENO);
// 		buff = get_next_line(STDIN_FILENO);
// 		printf("DEBUG: ft_read: buff: %s\n", buff);
// 		if (buff != NULL && !ft_isheredoc_lim(heredoc_lim, buff))
// 			write(fd, buff, ft_strlen(buff) + 1);
// 		else
// 			break;
// 	}
// }

static t_fd	ft_heredoc(t_data *data, char *heredoc_lim)
{
	t_fd	fd;

	if (heredoc_lim == NULL)
		return (-1);
	fd = open ("temp.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		ft_throw (data, ERR_FAIL, "file write", true);
	ft_read(data, fd, heredoc_lim);
	close (fd);
	fd = open ("temp.tmp", O_RDONLY, 0644);
	unlink ("temp.tmp");
	return (fd);
}

static void	ft_handlecmd_heredocs(t_data *data, int i)
{
	int		j;
	char	*fd;
	int		x;

	ft_assert_not_null (data, data);
	ft_assert_not_null (data, data->cmds[i].redirs);
	ft_assert_not_null (data, data->cmds[i].args_redir);
	j = -1;
	while (++j < data->cmds[i].redirsc)
	{
		if (data->cmds[i].redirs[j] == REDIR_HEREDOC)
		{
			x = -1;
			while (data->cmds[i].args_redir[j][++x] != '\0')
			if (data->cmds[i].args_redir[j][x] == '\"'
				|| data->cmds[i].args_redir[j][x] == '\'')
				ft_remove_quote(data, NULL, data->cmds[i].args_redir + j, x--);
			fd = ft_itoa (ft_heredoc (data,
						data->cmds[i].args_redir[j]));
			ft_assert_not_null (data, fd);
			free (data->cmds[i].args_redir[j]);
			data->cmds[i].args_redir[j] = fd;
		}
	}
}

void	ft_heredocs(t_data *data)
{
	int		i;

	ft_assert_not_null (data, data);
	ft_assert_not_null (data, data->cmds);
	i = -1;
	while (++i < data->cmdsc)
		if (data->cmds[i].redirsc > 0)
			ft_handlecmd_heredocs (data, i);
}
