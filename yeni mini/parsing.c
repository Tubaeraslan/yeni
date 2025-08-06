/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:18:28 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 17:05:11 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int add_arg(char **args, int index, char *token)
{
	args[index] = mem_absorb(ft_strdup(token));
	if (!args[index])
		return 0;
	return 1;
}

static int	check_and_handle_redirect(t_mini *mini, char **tokens, int *i)
{
	char	*tmp_token;

	tmp_token = tokens[*i];
	if (ft_strncmp(tmp_token, "<", 2) == 0 && tokens[*i + 1])
		return (handle_infile(mini, tokens, i));
	if (ft_strncmp(tmp_token, ">>", 3) == 0 && tokens[*i + 1])
		return (handle_outfile(mini, tokens, i, 1));
	if (ft_strncmp(tmp_token, ">", 2) == 0 && tokens[*i + 1])
		return (handle_outfile(mini, tokens, i, 0));
	if (ft_strncmp(tmp_token, "<<", 3) == 0 && tokens[*i + 1])
		return (handle_heredoc(mini, tokens, i));
	return (0);
}

static int	handle_tokens(t_mini *mini, char **tmp_args, int *count, int *i)
{
	char	*token;
	int		redirect_result;

	token = mini->tokens[*i];
	redirect_result = check_and_handle_redirect(mini, mini->tokens, i);
	if (redirect_result == 1)
		return (1);
	else if (redirect_result == -1)
	{
		mini->parsing_error = 1;
		mini->last_exit_code = 1;
		(*i)++;
		return (1);
	}
	if (ft_strncmp(token, "|", 2) == 0)
	{
		create_next_command(mini, mini->tokens, *i);
		return (2);
	}
	if (!add_arg(tmp_args, *count, token))
		return (-1);
	(*count)++;
	(*i)++;
	return (0);
}

static char	**alloc_args(int max_count)
{
	char	**args;
	int		i;

	args = mem_malloc(sizeof(char *) * (max_count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i <= max_count)
		args[i++] = NULL;
	return (args);
}

void	parsing(t_mini *mini)
{
	int		i;
	int		count;
	char	**tmp_args;
	int		result;

	i = 0;
	count = 0;
	tmp_args = alloc_args(mini->token_count);
	if (!tmp_args)
	{
		mini->last_exit_code = 1;
		return ;
	}
	while (i < mini->token_count)
	{
		result = handle_tokens(mini, tmp_args, &count, &i);
		if (result == 1)
			continue ;
		if (result == 2 || result == -1)
			break ;
	}
	tmp_args[count] = NULL;
	
	// Set cmd to the first argument (command name)
	if (count > 0 && tmp_args[0])
	{
		mini->cmd = mem_absorb(ft_strdup(tmp_args[0]));
	}
	else
	{
		mini->cmd = NULL;
	}
	
	// Set args array for execve
	mini->args = tmp_args;
	
	//update_args_and_cmd(mini, tmp_args, count);
}
