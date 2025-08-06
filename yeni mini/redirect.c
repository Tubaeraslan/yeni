/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:00:01 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 17:55:06 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redirection(t_mini *mini)
{
	int	fd;

	if (mini->is_heredoc)
	{
		mini->in_fd = mini->heredoc_fd;
		dup2(mini->heredoc_fd, STDIN_FILENO);
		close(mini->heredoc_fd);
		return (0);
	}
	if (mini->infile)
	{
		fd = open(mini->infile, O_RDONLY);
		if (fd < 0)
		{
			perror(mini->infile);
			return (-1);
		}
		mini->in_fd = fd;
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

static int	handle_output_redirection(t_mini *mini)
{
	int	fd;

	if (!mini->outfile)
		return (0);
	if (mini->is_append)
		fd = open(mini->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(mini->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(mini->outfile);
		return (-1);
	}
	mini->out_fd = fd;
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(t_mini *mini)
{
	if (handle_input_redirection(mini) < 0)
		return (-1);
	if (handle_output_redirection(mini) < 0)
		return (-1);
	return (0);
}