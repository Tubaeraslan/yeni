/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:12:15 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 18:40:51 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_input(t_mini *mini)
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
	{
		return;
	}
	if (syntax_error(mini)==0)
		return;
	expand_variables(mini);
	check_heredoc_and_setup(mini);
	if (!check_heredoc_conditions(mini))
		return;
	
	token(mini);
	parsing(mini);
}
