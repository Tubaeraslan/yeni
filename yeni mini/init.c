/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:06:07 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 13:39:51 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_fds(t_mini *mini)
{
	if (mini->heredoc_fd != -1)
		close(mini->heredoc_fd);
}

static void	init_minishell_part2(t_mini *mini)
{
	mini->is_heredoc = 0;
	mini->tokenizer.src = NULL;
	mini->tokenizer.i = 0;
	mini->tokenizer.j = 0;
	mini->tokenizer.inside_quotes = 0;
	mini->tokenizer.char_quote = '\0';
	mini->token_count = 0;
	mini->next = NULL;
	mini->is_pipe = 0;
	mini->infile = NULL;
	mini->outfile = NULL;
	mini->heredoc_limiter = NULL;
	mini->is_append = 0;
	mini->in_fd = -1;
	mini->out_fd = -1;
	mini->pids = NULL;
}

void	init_minishell(t_mini *mini, t_data *data, char **envp)
{
	mini->data = data;
	mini->input = NULL;
	mini->cmd = NULL;
	mini->args = NULL;
	mini->tokens = NULL;
	mini->parsing_error = 0;
	mini->error_printed = 0;
	mini->last_exit_code = 0;
	mini->heredoc_fd = -1;
	mini->heredocs = NULL;
	mini->data->env = ft_env_dup(envp);
	mini->data->export_list = mem_malloc(sizeof(char *));
	mini->data->export_list[0] = NULL;
	init_minishell_part2(mini);
}

static void	reset_mini_token_and_flags(t_mini *mini)
{
	mini->is_heredoc = 0;
	mini->tokenizer.src = NULL;
	mini->tokenizer.i = 0;
	mini->tokenizer.j = 0;
	mini->tokenizer.inside_quotes = 0;
	mini->tokenizer.char_quote = '\0';
	mini->token_count = 0;
	mini->next = NULL;
	mini->is_pipe = 0;
	mini->infile = NULL;
	mini->outfile = NULL;
	mini->heredoc_limiter = NULL;
	mini->is_append = 0;
	if (mini->pids)
	{
		mini->pids = NULL;
	}
}

void	reset_mini_for_new_command(t_mini *mini)
{
	if (mini->heredoc_fd != -1)
	{
		close(mini->heredoc_fd);
		mini->heredoc_fd = -1;
	}
	if (mini->in_fd != -1)
	{
		close(mini->in_fd);
		mini->in_fd = -1;
	}
	if (mini->out_fd != -1)
	{
		close(mini->out_fd);
		mini->out_fd = -1;
	}
	mini->input = NULL;
	mini->cmd = NULL;
	mini->args = NULL;
	mini->tokens = NULL;
	mini->parsing_error = 0;
	mini->error_printed = 0;
	mini->heredocs = NULL;
	reset_mini_token_and_flags(mini);
}
