/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:15:13 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 13:04:55 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (src[i] && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	while (i < len)
	{
		dst[i] = '\0';
		i++;
	}
	return (dst);
}

char	*extract_limiter(char *input, int i)
{
	int		start;
	int		len;
	char	*limiter;

	while (input[i] == ' ' || input[i] == '\t')
		i++;
	start = i;
	while (input[i] && input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
		i++;
	len = i - start;
	if (len == 0)
		return (NULL);
	limiter = mem_malloc(len + 1);
	ft_strncpy(limiter, input + start, len);
	limiter[len] = '\0';
	return (limiter);
}

t_heredoc	*get_all_heredocs(char *input)
{
	t_heredoc	*head;
	t_heredoc	*last;
	int			i;
	int			in_squote;
	int			in_dquote;

	head = NULL;
	last = NULL;
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (input[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		if (!in_squote && !in_dquote
			&& input[i] == '<' && input[i + 1] == '<')
			add_heredoc_node(&head, &last, input, &i);
		else
			i++;
	}
	return (head);
}

static int	is_heredoc(char *input)
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

void	check_heredoc_and_setup(t_mini *mini)
{
	t_mini		*tmp;
	t_heredoc	*heredocs;

	tmp = mini;
	while (tmp)
	{
		if (is_heredoc(tmp->input) && tmp->heredoc_fd == -1)
		{
			heredocs = get_all_heredocs(tmp->input);
			tmp->heredocs = heredocs;
			process_heredoc_list(tmp, heredocs);
			if (g_signal_status == 130)
				return ;
		}
		tmp = tmp->next;
	}
	signal(SIGINT, sigint_handler);
}
