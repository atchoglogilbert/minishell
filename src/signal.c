/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:34:46 by rburgsta          #+#    #+#             */
/*   Updated: 2023/01/04 07:18:29 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>
#include <stdlib.h>

void signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n\033[36;1mminishell >\033[0m", 24);
		rl_replace_line("", 0);
	}
	else if (sig == SIGQUIT)
		write(1, "SIGQUIT recieved\n", 18);
}

int main()
{
	char *input = NULL;

	struct sigaction	sa;

	sa.sa_handler = &signal_handler;
	sa.sa_flags = SA_RESTART;
	printf("PID: %i\n", getpid());
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	while (1)
	{
		input = readline("\033[36;1mminishell >\033[0m");
		if (input == NULL)
		{
			printf("%c%cexit\n", 8, 8);
			//system("leaks a.out");
			exit(0);
		}
		else if (input[0] != '\0')
			add_history(input);
		printf("'%s'\n", input);
		free(input);
	}
}