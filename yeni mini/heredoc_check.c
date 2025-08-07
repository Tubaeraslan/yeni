/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:47:18 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 13:04:22 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_heredoc(char *input)
{
	int	i;
	int	inside_squote;
	int	inside_dquote;

	i = 0;
	inside_squote = 0;
	inside_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !inside_dquote)
			inside_squote = !inside_squote;
		else if (input[i] == '\"' && !inside_squote)
			inside_dquote = !inside_dquote;
		else if (!inside_squote && !inside_dquote)
		{
			if (input[i] == '<' && input[i + 1] == '<')
				return (1);
		}
		i++;
	}
	return (0);
}

int	has_any_heredoc(t_mini *mini)
{
	t_mini	*cur;

	cur = mini;
	while (cur)
	{
		if (is_heredoc(cur->input))
			return (1);
		cur = cur->next;
	}
	return (0);
}

static void	handle_heredoc_interrupt(t_mini *mini)
{
	mini->last_exit_code = 130;
	g_signal_status = 0;
	if (mini->input)
	{
		mini->input = NULL;
	}
	mini->is_heredoc = 0;
	mini->heredoc_fd = -1;
}

int	check_heredoc_conditions(t_mini *mini)
{
	if (g_signal_status == 130)
	{
		handle_heredoc_interrupt(mini);
		return (0);
	}
	if (has_any_heredoc(mini) && mini->heredoc_fd == -1)
	{
		handle_heredoc_interrupt(mini);
		return (0);
	}
	return (1);
}
