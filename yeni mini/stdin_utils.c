/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:31:34 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 17:23:58 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_heredoc_setup(t_mini *mini)
{
	if (dup2(mini->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		ft_exit_gc(1);
	}
	close(mini->heredoc_fd);
	mini->heredoc_fd = -1;
}

static void	handle_infile_setup(t_mini *mini)
{
	int	fd_in;

	if (access(mini->infile, F_OK) != 0)
	{
		perror(mini->infile);
		ft_exit_gc(1);
	}
	fd_in = open(mini->infile, O_RDONLY);
	if (fd_in < 0)
	{
		perror(mini->infile);
		ft_exit_gc(1);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2 infile");
		ft_exit_gc(1);
	}
	close(fd_in);
}

static void	handle_prev_fd(int prev_fd, t_mini *mini)
{
	(void)mini;
	if (dup2(prev_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 prev_fd");
		ft_exit_gc(1);
	}
	close(prev_fd);
}

void	setup_stdout(t_mini *mini, int *fd)
{
	if (mini->next && fd)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 fd[1]");
			ft_exit_gc(1);
		}
		close(fd[1]);
	}
}

void	setup_stdin(t_mini *mini, int prev_fd)
{
	if (mini->is_heredoc)
		handle_heredoc_setup(mini);
	else if (mini->infile)
		handle_infile_setup(mini);
	else if (prev_fd != -1)
		handle_prev_fd(prev_fd, mini);
}
