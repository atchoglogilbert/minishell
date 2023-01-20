/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:34:46 by rburgsta          #+#    #+#             */
/*   Updated: 2023/01/20 11:08:04 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_signal_handler(int sig, siginfo_t *info, void *ptr)
{
	static t_data *data;

	(void)info;
	if (data == NULL)
		data = (t_data *)ptr;
	if (sig == SIGINT)
		write(1, "\n", 1);
	if (sig == SIGINT)
	{
		if (data->read_state)
		{
			rl_on_new_line ();
			rl_replace_line ("", 0);
			rl_redisplay ();
		}
		// else 
		// {
		// 	rl_replace_line ("\03", 0);
		// 	rl_on_new_line ();
		// 	rl_redisplay ();
		// 	//ft_putchar_fd('\04', 0);
		// }
		data->status = EXIT_SIGINT;
	}
	// else if (sig == SIGQUIT)
	// {
	// 	rl_on_new_line();
	// 	rl_redisplay();
	// }
}

void	ft_signal_handler_child(int sig)
{
	if (sig == SIGINT)
		exit(2);
	else if (sig == SIGQUIT)
	{
		write(1, "Quit: 3\n", 8);
		exit(3);
	}
}

void	ft_init_signal_handler(t_data *data)
{
	struct sigaction	sa;

	data->tty_attr.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSADRAIN, &data->tty_attr) != 0)
		ft_throw(data, ERR_FAIL, "main settattr fail", true);
	ft_signal_handler(0, NULL, data);
	sa.sa_sigaction = &ft_signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) || signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_throw(data, ERR_FAIL, "set_signal", true);
}
