/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:03:36 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 14:13:39 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_prepare(t_mini *mini, char *cwd, char **oldpwd, char **path)
{
	size_t	cwd_size;

	cwd_size = 1024;
	if (get_oldpwd(mini, cwd, cwd_size, oldpwd))
		return (1);
	if (get_path(mini, path))
		return (1);
	return (0);
}

static int	cd_change_dir(t_mini *mini, char *path, char **newcwd)
{
	char	cwd[1024];

	if (chdir(path) != 0)
	{
		perror("cd");
		mini->last_exit_code = 1;
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		mini->last_exit_code = 1;
		return (1);
	}
	*newcwd = mem_absorb(ft_strdup(cwd));
	if (!*newcwd)
	{
		perror("strdup");
		mini->last_exit_code = 1;
		return (1);
	}
	return (0);
}

static int	update_pwd_oldpwd_env(t_mini *mini, char *oldpwd, char *newcwd)
{
	char	*oldpwd_var;
	char	*pwd_var;

	oldpwd_var = mem_absorb(ft_strjoin("OLDPWD=", oldpwd));
	pwd_var = mem_absorb(ft_strjoin("PWD=", newcwd));
	if (!oldpwd_var || !pwd_var)
	{
		free(oldpwd_var);
		free(pwd_var);
		mini->last_exit_code = 1;
		return (1);
	}
	update_env(&mini->data->env, oldpwd_var);
	update_env(&mini->data->env, pwd_var);
	return (0);
}

static int	cd_chdir_and_update_env(t_mini *mini, char *path, char *oldpwd)
{
	char	*newcwd;

	if (cd_change_dir(mini, path, &newcwd))
	{
		return (1);
	}
	if (update_pwd_oldpwd_env(mini, oldpwd, newcwd))
	{
		return (1);
	}
	return (0);
}

void	ft_cd(t_mini *mini)
{
	int		arg_count;
	char	*path;
	char	cwd[1024];
	char	*oldpwd;

	oldpwd = NULL;
	arg_count = 0;
	while (mini->args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		mini->last_exit_code = 1;
		return ;
	}
	if (cd_prepare(mini, cwd, &oldpwd, &path))
		return ;
	if (cd_chdir_and_update_env(mini, path, oldpwd))
		return ;
	mini->last_exit_code = 0;
}
