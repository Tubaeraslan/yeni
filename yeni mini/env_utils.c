/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:08:20 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 14:35:20 by teraslan         ###   ########.fr       */
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

char **add_to_env(char **env, char *new_var)
{
	int i = 0;
	char **new_env;

	// Mevcut ortamın eleman sayısını say
	while (env && env[i])
		i++;

	// Yeni ortam için 1 fazla yer ayır (yeni değişken + NULL)
	new_env = mem_absorb((char **)malloc(sizeof(char *) * (i + 2)));
	if (!new_env)
		return (NULL);

	// Eski ortamı kopyala
	i = 0;
	while (env && env[i])
	{
		new_env[i] = mem_absorb(ft_strdup(env[i]));
		i++;
	}

	// Yeni değişkeni ekle
	new_env[i] = mem_absorb(ft_strdup(new_var));
	new_env[i + 1] = NULL;


	return (new_env);
}

void update_env(char ***envp, char *arg)
{
	int i = 0;
	int key_len = 0;
	char *key;

	while (arg[key_len] && arg[key_len] != '=')
		key_len++;
	key = mem_absorb(ft_substr(arg, 0, key_len));

	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, key_len) == 0 && (*envp)[i][key_len] == '=')
		{
			(*envp)[i] = mem_absorb(ft_strdup(arg));
			return;
		}
		i++;
	}

	// yoksa yeni değişkeni ekle
	*envp = add_to_env(*envp, arg); // realloc + strdup ile eklenir
}