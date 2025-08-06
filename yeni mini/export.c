/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:22:02 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 14:36:13 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_and_declare(char **sorted_env)
{
	int	i;

	i = 0;
	while (sorted_env[i])
	{
		ft_putstr_fd("declare -x ", 1);
		print_declare_line(sorted_env[i]);
		i++;
	}
}

static void	print_export_exclusive(char **sorted_export, char **sorted_env)
{
	int	i;

	i = 0;
	while (sorted_export[i])
	{
		if (!find_in_env(sorted_env, sorted_export[i]))
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(sorted_export[i], 1);
			write(1, "\n", 1);
		}
		i++;
	}
}

static void	print_export_list(char **env, char **export_list)
{
	char	**sorted_env;
	char	**sorted_export;

	sorted_env = sort_env(env);
	sorted_export = sort_env(export_list);
	print_env_and_declare(sorted_env);
	print_export_exclusive(sorted_export, sorted_env);
}

static int	ft_export_handle_arg(t_mini *mini, char *arg)
{
	if (!is_valid(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (ft_strchr(arg, '='))
		update_env(&mini->data->env, arg);
	else
		update_export(mini, mem_absorb(ft_strdup(arg)));
	return (0);
}

void	ft_export(t_mini *mini)
{
	int		i;
	int		error_flag;
	char	**args;

	i = 1;
	error_flag = 0;
	args = mini->args;
	if (!mini->args[i])
	{
		print_export_list(mini->data->env, mini->data->export_list);
		mini->last_exit_code = 0;
		return ;
	}
	while (mini->args[i])
	{
		if (ft_export_handle_arg(mini, args[i]))
			error_flag = 1;
		i++;
	}
	if (error_flag)
		mini->last_exit_code = 1;
	else
		mini->last_exit_code = 0;
}
