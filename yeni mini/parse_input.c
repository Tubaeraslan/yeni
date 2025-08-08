/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:12:15 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 12:48:28 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_tokens_and_fill_args(t_mini *mini, char **tmp_args)
{
	int	i;
	int	count;
	int	result;

	i = 0;
	count = 0;
	while (i < mini->token_count)
	{
		result = handle_tokens(mini, tmp_args, &count, &i);
		if (result == 1)
			continue ;
		if (result == 2 || result == -1)
			break ;
	}
	tmp_args[count] = NULL;
	if (count > 0 && tmp_args[0])
		mini->cmd = mem_absorb(ft_strdup(tmp_args[0]));
	else
		mini->cmd = NULL;
	return (0);
}

void	parse_input(t_mini *mini)
{
	mini->tokens = NULL;
	mini->token_count = 0;
	if (ft_strncmp(mini->input, "\"\"", 2) == 0)
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		mini->last_exit_code = 127;
		return ;
	}
	if (mini->input == NULL)
		return ;
	if (syntax_error(mini) == 0)
		return ;
	expand_variables(mini);
	check_heredoc_and_setup(mini);
	if (!check_heredoc_conditions(mini))
		return ;
	token(mini);
	parsing(mini);
}
