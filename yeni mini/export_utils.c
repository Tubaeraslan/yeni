/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:26:16 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 14:38:07 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_in_env(char **env, char *var)
{
	int		i;
	size_t	var_len;

	var_len = strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	is_in_list(char **list, char *var)
{
	int	i;

	i = 0;
	if (!var)
		return (0);
	if (!list)
		return (0);
	while (list[i])
	{
		if (!list[i])
			break ;
		if (ft_strncmp(list[i], var, ft_strlen(var)) == 0 && ft_strlen(list[i]) == ft_strlen(var))
			return (1);
		i++;
	}
	return (0);
}

void	extend_export_list(t_mini *mini, char *var)
{
	int		i;
	char	**new_list;
	int		j;

	i = 0;
	j = 0;
	while (mini->data->export_list && mini->data->export_list[i])
		i++;
	new_list = mem_malloc(sizeof(char *) * (i + 2));
	while (j < i)
	{
		new_list[j] = mini->data->export_list[j];
		j++;
	}
	new_list[i] = var;
	new_list[i + 1] = NULL;
	if (mini->data->export_list)
		mini->data->export_list = NULL;
	mini->data->export_list = new_list;
}

void	update_export(t_mini *mini, char *var)
{
	if (is_in_list(mini->data->export_list, var))
	{
		free(var);
		return ;
	}
	extend_export_list(mini, var);
}

void	print_declare_line(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		write(1, &env[i++], 1);
	if (env[i] == '=')
	{
		write(1, "=\"", 2);
		ft_putstr_fd(&env[i + 1], 1);
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}
