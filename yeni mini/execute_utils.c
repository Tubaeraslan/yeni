/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:02:02 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 13:45:11 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_fork_step(t_mini *mini, pid_t *pids, int *i, int *prev_fd)
{
	int		fd[2];

	if (mini->next && pipe(fd) == -1)
		return (perror("pipe"), -1);
	if (mini->next)
		pids[(*i)++] = handle_fork(mini, *prev_fd, fd);
	else
		pids[(*i)++] = handle_fork(mini, *prev_fd, NULL);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (mini->next)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
	else
		*prev_fd = -1;
	return (0);
}

void	closer(int prev_fd)
{
	if (dup2(prev_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 prev_fd");
		ft_exit_gc(1);
	}
	close(prev_fd);
}
