/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:05:34 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 14:13:17 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_oldpwd(t_mini *mini, char *cwd, size_t c_size, char **oldpwd)
{
	if (!getcwd(cwd, c_size))
	{
		perror("getcwd");
		mini->last_exit_code = 1;
		return (1);
	}
	*oldpwd = mem_absorb(ft_strdup(cwd));
	if (!*oldpwd)
	{
		perror("strdup");
		mini->last_exit_code = 1;
		return (1);
	}
	return (0);
}

int	get_path(t_mini *mini, char **path)
{
	if (!mini->args[1] || ft_strncmp(mini->args[1], "~", 2) == 0)
	{
		*path = getenv("HOME");
		if (!*path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			mini->last_exit_code = 1;
			return (1);
		}
	}
	else
		*path = mini->args[1];
	return (0);
}
