/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:08:20 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 12:53:44 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_len(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

static void	bubble_sort(char **arr, int len)
{
	int		i;
	int		j;
	char	*tmp;
	size_t	n;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			n = ft_strlen(arr[j]);
			if (ft_strlen(arr[j + 1]) > n)
				n = ft_strlen(arr[j + 1]);
			if (ft_strncmp(arr[j], arr[j + 1], n + 1) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	**sort_env(char **env)
{
	int		len;
	char	**sorted;

	len = env_len(env);
	sorted = ft_env_dup(env);
	if (!sorted)
		return (NULL);
	bubble_sort(sorted, len);
	return (sorted);
}

char	**add_to_env(char **env, char *new_var)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env && env[i])
		i++;
	new_env = mem_absorb((char **)malloc(sizeof(char *) * (i + 2)));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env && env[i])
	{
		new_env[i] = mem_absorb(ft_strdup(env[i]));
		i++;
	}
	new_env[i] = mem_absorb(ft_strdup(new_var));
	new_env[i + 1] = NULL;
	return (new_env);
}

void	update_env(char ***envp, char *arg)
{
	int		i;
	int		key_len;
	char	*key;

	i = 0;
	key_len = 0;
	while (arg[key_len] && arg[key_len] != '=')
		key_len++;
	key = mem_absorb(ft_substr(arg, 0, key_len));
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, key_len) == 0
			&& (*envp)[i][key_len] == '=')
		{
			(*envp)[i] = mem_absorb(ft_strdup(arg));
			return ;
		}
		i++;
	}
	*envp = add_to_env(*envp, arg);
}
