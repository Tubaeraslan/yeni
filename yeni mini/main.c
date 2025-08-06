/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:46:18 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 18:31:13 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal_status = 0;

void ft_exit_gc(int status)
{
	mem_free();
	exit(status);
}

char	**ft_env_dup(char **envp)
{
	int		i;
	char	**env_copy;

	i = 0;
	while (envp && envp[i])
		i++;
	env_copy = mem_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp && envp[i])
	{
		env_copy[i] = mem_absorb(ft_strdup(envp[i]));
		if (!env_copy[i])
		{
			return (NULL);
		}
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
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

void	reset_mini_for_new_command(t_mini *mini)
{
	// Close any open file descriptors from previous command
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

static int	read_and_prepare_input(t_mini *mini)
{
	char	*input;

	input = readline("minishell$ ");
	if (!input)
	{
		printf("exit\n");
		return (1);
	}
	add_history(input);
	mini->input = mem_absorb(ft_strdup(input));
	free(input);
	return (0);
}

void ft_close_fds(t_mini *mini)
{
	if (mini->heredoc_fd != -1)
		close(mini->heredoc_fd);
}

int main(int argc, char **argv,char **envp)
{
	t_mini	*mini;
	t_data	*data;
	
	(void)argc; // Unused parameter
	(void)argv; // Unused parameter
	mini = mem_malloc(sizeof(t_mini));
	data = mem_malloc(sizeof(t_data));
	init_minishell(mini,data, envp);
	handle_signals();
	while (1)
	{
		g_signal_status = 0;
		reset_mini_for_new_command(mini);
		if (read_and_prepare_input(mini))
			break ;
		if (g_signal_status != 0)
		{
			mini->last_exit_code = g_signal_status;
			g_signal_status = 0;
		}
		parse_input(mini);
		execute_commands(mini);
	}
	ft_close_fds(mini);
	mem_free();
	return 0;
}
