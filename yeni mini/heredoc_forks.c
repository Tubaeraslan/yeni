/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_forks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:20:59 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 18:24:15 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parent_heredoc_process(t_mini *mini, int pipe_fd[2], pid_t pid)
{
	int	status;

	set_signal(1);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (mini->heredoc_fd != -1)
			close(mini->heredoc_fd);
		close(pipe_fd[0]);
		return (0);
	}
	if (mini->heredoc_fd != -1)
	{
		close(mini->heredoc_fd);
		mini->heredoc_fd = -1;
	}
	mini->heredoc_fd = pipe_fd[0];
	mini->is_heredoc = 1;
	return (1);
}

static void	child_heredoc_process(t_mini *mini, char *limiter, int pipe_fd[2])
{
	int	fd;

	(void)mini;
	close(pipe_fd[0]);
	fd = 3;
	while (fd < 1024)
	{
		if (fd != pipe_fd[1])
			close(fd);
		fd++;
	}
	set_signal(0);
	heredoc_loop_custom(limiter, pipe_fd[1]);
	close(pipe_fd[1]);
	ft_exit_gc(0);
}

static int	fork_and_handle_heredoc(t_mini *mini, char *limiter)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		mem_free();
		ft_exit_gc(1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		mem_free();
		ft_exit_gc(1);
	}
	if (pid == 0)
		child_heredoc_process(mini, limiter, pipe_fd);
	return (parent_heredoc_process(mini, pipe_fd, pid));
}

void	process_heredoc_list(t_mini *mini, t_heredoc *heredocs)
{
	t_heredoc	*tmp;

	tmp = heredocs;
	while (tmp)
	{
		if (!fork_and_handle_heredoc(mini, tmp->limiter))
		{
			mini->heredoc_fd = -1;
			mini->is_heredoc = 0;
			g_signal_status = 130;
			mini->heredocs = NULL;
			return ;
		}
		tmp = tmp->next;
	}
}

void	add_heredoc_node(t_heredoc **h, t_heredoc **l, char *input, int *i)
{
	t_heredoc	*node;

	node = mem_malloc(sizeof(t_heredoc));
	if (!node)
		return ;
	node->limiter = extract_limiter(input, *i + 2);
	node->index = *i;
	node->next = NULL;
	if (!*h)
		*h = node;
	else
		(*l)->next = node;
	*l = node;
	*i += 2;
}
