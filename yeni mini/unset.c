/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:02:06 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 12:37:21 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**remove_from_env(char **env, const char *var)
{
	int		i;
	int		j;
	int		count;
	char	**new_env;
	int		len;

	i = 0;
	j = 0;
	count = 0;
	len = ft_strlen(var);
	while (env && env[count])
		count++;
	new_env = mem_malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		if (!(ft_strncmp(env[i], var, len) == 0 && env[i][len] == '='))
			new_env[j++] = mem_absorb(ft_strdup(env[i]));
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

static char	**remove_from_exp(char **export_list, const char *var)
{
	int		i;
	int		j;
	int		count;
	char	**new_list;
	int		len;

	if (!export_list)
		return (NULL);
	len = ft_strlen(var);
	count = 0;
	while (export_list[count])
		count++;
	new_list = mem_malloc(sizeof(char *) * (count + 1));
	if (!new_list)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < count)
		if (!(ft_strncmp(export_list[i], var, len) == 0
				&& (export_list[i][len] == '=' || export_list[i][len] == '\0')))
			new_list[j++] = mem_absorb(ft_strdup(export_list[i]));
	new_list[j] = NULL;
	return (new_list);
}

void	ft_unset(t_mini *mini)
{
	int	i;

	i = 1;
	while (mini->args[i])
	{
		if (is_valid(mini->args[i]))
		{
			mini->data->env = remove_from_env(mini->data->env, mini->args[i]);
			mini->data->export_list = remove_from_exp(mini->data->export_list,
					mini->args[i]);
		}
		i++;
	}
	mini->last_exit_code = 0;
}
