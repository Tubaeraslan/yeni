/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:56:19 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/05 19:59:30 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_operators(t_mini *mini, char *src)
{
	if (src[0] == '>' && src[1] == '>')
		return (add_token(mini, mem_absorb(ft_strdup(">>"))), 2);
	if (src[0] == '<' && src[1] == '<')
		return (add_token(mini, mem_absorb(ft_strdup("<<"))), 2);
	if (src[0] == '>')
		return (add_token(mini, mem_absorb(ft_strdup(">"))), 1);
	if (src[0] == '<')
		return (add_token(mini, mem_absorb(ft_strdup("<"))), 1);
	if (src[0] == '|')
		return (add_token(mini, mem_absorb(ft_strdup("|"))), 1);
	return (0);
}

int	process_quotes(t_tokenizer *tk)
{
	const char	*src;
	int			*i;

	src = tk->src;
	i = &tk->i;
	if (!tk->inside_quotes)
	{
		tk->inside_quotes = 1;
		tk->char_quote = src[*i];
		(*i)++;
		return (1);
	}
	else if (src[*i] == tk->char_quote)
	{
		tk->inside_quotes = 0;
		tk->char_quote = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

void	process_space(t_mini *mini, t_tokenizer *tk)
{
	if (tk->j > 0)
	{
		tk->buffer[tk->j] = '\0';
		add_token(mini, mem_absorb(ft_strdup(tk->buffer)));
		tk->j = 0;
	}
}

int	process_operator(t_mini *mini, t_tokenizer *tk)
{
	if (tk->j > 0)
	{
		tk->buffer[tk->j] = '\0';
		add_token(mini, mem_absorb(ft_strdup(tk->buffer)));
		tk->j = 0;
	}
	return (handle_operators(mini, (char *)&tk->src[tk->i]));
}