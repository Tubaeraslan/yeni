/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:23:41 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 18:18:39 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(t_mini *mini)
{
	int	count;

	count = 0;
	while (mini)
	{
		count++;
		mini = mini->next;
	}
	return (count);
}

int	validate_all_commands(t_mini *mini)
{
	t_mini	*current;
	char		*path;

	current = mini;
	while (current)
	{
		if (current->cmd && !is_built(current->cmd))
		{
			path = path_finder(current->cmd, current->data->env);
			if (!path)
			{
				ft_putstr_fd("command not found: ", 2);
				ft_putstr_fd(current->cmd, 2);
				ft_putchar_fd('\n', 2);
				return (127);
			}
		}
		current = current->next;
	}
	return (0);
}

static int	get_exit_code_from_status(int status)
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

static int	wait_for_children(pid_t *pids, int count, pid_t last_pid)
{
	int		status;
	int		exit_code;
	int		waited;
	pid_t	pid;

	exit_code = 0;
	waited = 0;
	(void)pids;
	while (waited < count)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == last_pid)
			exit_code = get_exit_code_from_status(status);
		waited++;
	}
	return (exit_code);
}

int	execute_pipeline(t_mini *mini, pid_t *pids)
{
	int		fd[2];
	int		prev_fd;
	int		i;
	pid_t	last_pid;

	prev_fd = -1;
	i = 0;
	last_pid = -1;
	while (mini)
	{
		if (mini->next && pipe(fd) == -1)
			return (perror("pipe"), -1);
		if (mini->next)
			pids[i++] = handle_fork(mini, prev_fd, fd);
		else
			pids[i++] = handle_fork(mini, prev_fd, NULL);
		last_pid = pids[i - 1];
		if (prev_fd != -1)
			close(prev_fd);
		if (mini->next)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		else
		{
			prev_fd = -1;
			if (prev_fd != -1)
				close(prev_fd);
		}
		mini = mini->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (wait_for_children(pids, i, last_pid));
}