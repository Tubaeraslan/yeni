/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:43:10 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 12:25:55 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_exit_code(t_expand *ex, t_mini *mini)
{
	char	*exit_str;
	int		k;

	exit_str = mem_absorb(ft_itoa(mini->last_exit_code));
	k = 0;
	while (exit_str[k])
		ex->new_input[ex->j++] = exit_str[k++];
	ex->i++;
}

void	expand_variable_from_env(t_expand *ex, t_mini *mini)
{
	char	varname[256];
	int		len;
	char	*value;

	ex->i++;
	if (ex->input[ex->i] == '?')
	{
		expand_exit_code(ex, mini);
		return ;
	}
	else if (ex->input[ex->i] == '$')
	{
		expand_pid(ex);
		return ;
	}
	len = read_varname(ex->input, &(ex->i), varname);
	if (len == 0)
	{
		ex->new_input[ex->j++] = '$';
		return ;
	}
	value = get_env_value(mini->data->env, varname);
	if (value)
		copy_env_value_to_input(ex, value);
}

void	expand_loop(t_expand *ex, t_mini *mini)
{
	while (ex->input[ex->i])
	{
		if (ex->input[ex->i] == '\'' && !ex->double_quote)
		{
			ex->single_quote = !ex->single_quote;
			ex->new_input[ex->j++] = ex->input[ex->i++];
		}
		else if (ex->input[ex->i] == '"' && !ex->single_quote)
		{
			ex->double_quote = !ex->double_quote;
			ex->new_input[ex->j++] = ex->input[ex->i++];
		}
		else if (ex->input[ex->i] == '$' && !ex->single_quote)
			expand_variable_from_env(ex, mini);
		else
			ex->new_input[ex->j++] = ex->input[ex->i++];
	}
}

void	expand_variables(t_mini *mini)
{
	t_expand	*ex;

	ex = mem_malloc(sizeof(t_expand));
	if (!ex)
		return ;
	ex->input = mini->input;
	ex->new_input = mem_malloc(4096);
	if (!ex->new_input)
		return ;
	ex->i = 0;
	ex->j = 0;
	ex->single_quote = 0;
	ex->double_quote = 0;
	expand_loop(ex, mini);
	ex->new_input[ex->j] = '\0';
	mini->input = mem_absorb(ft_strdup(ex->new_input));
	ex->new_input = NULL;
}