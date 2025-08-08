/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:07:17 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 15:10:46 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_code_from_status(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\r", 1);
			return (130);
		}
		else if (sig == SIGQUIT)
		{
			write(1, "Quit (core dumped)\n", 19);
			return (131);
		}
		else
			return (128 + sig);
	}
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	numeric_control(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (arg[i] == '\0')
		return (1);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	return (0);
}

void	close_all_fds(t_mini *mini)
{
	while (mini)
	{
		if (mini->in_fd > 2)
		{
			close(mini->in_fd);
			mini->in_fd = -1;
		}
		if (mini->out_fd > 2)
		{
			close(mini->out_fd);
			mini->out_fd = -1;
		}
		if (mini->heredoc_fd > 2)
		{
			close(mini->heredoc_fd);
			mini->heredoc_fd = -1;
		}
		mini = mini->next;
	}
}

void	exit_program(t_mini *mini, int exit_code)
{
	close_all_fds(mini);
	rl_clear_history();
	ft_exit_gc(exit_code);
}

int	ft_exit(t_mini *mini)
{
	long	exit_code;

	write(2, "exit\n", 5);
	exit_control(mini);
	if (mini->args[1] && mini->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (mini->args[1] && numeric_control(mini->args[1]) == 1)
		exit_control2(mini);
	if (mini->args[1])
	{
		exit_code = ft_atoi(mini->args[1]);
		if (exit_code < 0)
			exit_code = 256 + (exit_code % 256);
		exit_program(mini, (int)(exit_code % 256));
	}
	else
		exit_program(mini, 0);
	return (0);
}
