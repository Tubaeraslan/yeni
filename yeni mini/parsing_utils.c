/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:21:06 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 13:16:51 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_infile(t_mini *mini, char **tokens, int *i)
{
	char	*filename;

	filename = tokens[*i + 1];
	if (mini->parsing_error)
	{
		*i += 2;
		return (1);
	}
	if (access(filename, F_OK) != 0)
	{
		perror(filename);
		mini->parsing_error = 1;
		mini->last_exit_code = 1;
		*i += 2;
		return (1);
	}
	if (mini->infile)
		mini->infile = NULL;
	mini->infile = mem_absorb(ft_strdup(filename));
	*i += 2;
	return (1);
}

int	handle_outfile(t_mini *mini, char **tokens, int *i, int append)
{
	char	*filename;
	int		fd;

	filename = tokens[*i + 1];
	if (mini->parsing_error)
	{
		*i += 2;
		return (1);
	}
	if (append)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(filename);
		mini->parsing_error = 1;
		mini->last_exit_code = 1;
	}
	else
		close(fd);
	mini->outfile = mem_absorb(ft_strdup(filename));
	mini->is_append = append;
	*i += 2;
	return (1);
}

int	handle_heredoc(t_mini *mini, char **tokens, int *i)
{
	if (mini->heredoc_limiter)
		free(mini->heredoc_limiter);
	mini->heredoc_limiter = mem_absorb(ft_strdup(tokens[*i + 1]));
	if (mini->infile)
	{
		mini->infile = NULL;
	}
	mini->is_heredoc = 1;
	*i += 2;
	return (1);
}

t_mini	*create_next_command(t_mini *mini, char **tokens, int index)
{
	int		pipe_token_count;
	t_mini	*next_mini;
	int		j;

	j = 0;
	pipe_token_count = mini->token_count - (index + 1);
	if (pipe_token_count <= 0)
		return (NULL);
	next_mini = mem_malloc(sizeof(t_mini));
	ft_memset(next_mini, 0, sizeof(t_mini));
	next_mini->tokens = mem_malloc(sizeof(char *) * pipe_token_count);
	while (j < pipe_token_count)
	{
		next_mini->tokens[j] = mem_absorb(ft_strdup(tokens[index + 1 + j]));
		j++;
	}
	next_mini->token_count = pipe_token_count;
	next_mini->data = mini->data;
	next_mini->input = mem_absorb(ft_strdup(tokens[index + 1]));
	mini->input = mem_absorb(ft_strdup(tokens[0]));
	mini->token_count = index;
	mini->next = next_mini;
	mini->is_pipe = 1;
	parsing(next_mini);
	return (next_mini);
}
