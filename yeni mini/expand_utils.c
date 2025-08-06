/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:51:43 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 12:25:45 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char **env, char *key)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

void	copy_env_value_to_input(t_expand *ex, char *value)
{
	int	m;

	m = 0;
	while (value[m])
		ex->new_input[ex->j++] = value[m++];
}

void	expand_pid(t_expand *ex)
{
	char	*pid_str;
	int		m;

	pid_str = mem_absorb(ft_itoa(getpid()));
	if (!pid_str)
		return ;
	m = 0;
	while (pid_str[m])
		ex->new_input[ex->j++] = pid_str[m++];
	ex->i++;
}

int	read_varname(char *input, int *i, char *varname)
{
	int	k;

	k = 0;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		varname[k++] = input[*i];
		(*i)++;
	}
	varname[k] = '\0';
	return (k);
}
