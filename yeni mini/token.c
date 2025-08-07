/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:38:18 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 13:49:32 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_mini *mini, char *buffer)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = mem_malloc(sizeof(char *) * (mini->token_count + 2));
	if (!tmp)
	{
		perror("malloc failed");
		mem_free();
		ft_exit_gc(1);
	}
	while (i < mini->token_count)
	{
		tmp[i] = mem_absorb(ft_strdup(mini->tokens[i]));
		i++;
	}
	tmp[i++] = buffer;
	tmp[i] = NULL;
	mini->tokens = tmp;
	mini->token_count++;
}

static void	process_remaining_buffer(t_mini *mini, t_tokenizer *tk)
{
	if (tk->j > 0)
	{
		tk->buffer[tk->j] = '\0';
		add_token(mini, mem_absorb(ft_strdup(tk->buffer)));
		tk->j = 0;
	}
}

static void	process_char(t_tokenizer *tk, t_mini *mini)
{
	const char	*src;
	int			*i;
	int			len;

	src = tk->src;
	i = &tk->i;
	if (src[*i] == '\'' || src[*i] == '"')
	{
		if (process_quotes(tk))
			return ;
	}
	else if (!tk->inside_quotes && (src[*i] == ' ' || src[*i] == '\t'))
	{
		process_space(mini, tk);
		(*i)++;
		return ;
	}
	else if (!tk->inside_quotes && (src[*i] == '>'
			|| src[*i] == '<' || src[*i] == '|'))
	{
		len = process_operator(mini, tk);
		(*i) += len;
		return ;
	}
	tk->buffer[(tk->j)++] = src[(*i)++];
}

void	token(t_mini *mini)
{
	t_tokenizer	*tk;

	tk = &mini->tokenizer;
	tk->i = 0;
	tk->j = 0;
	tk->inside_quotes = 0;
	tk->char_quote = 0;
	tk->src = mini->input;
	while (tk->src[tk->i])
		process_char(tk, mini);
	process_remaining_buffer(mini, tk);
}
