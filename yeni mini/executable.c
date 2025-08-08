/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:11:41 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 15:10:38 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_executable_path(char **paths, char *cmd)
{
	char	*path;
	char	*temp;
	int		i;
	int		j;

	i = 0;
	while (paths[i])
	{
		temp = mem_absorb(ft_strjoin(paths[i], "/"));
		path = mem_absorb(ft_strjoin(temp, cmd));
		if (access(path, X_OK) == 0)
		{
			j = 0;
			while (paths[j])
			{
				free(paths[j]);
				j++;
			}
			free(paths);
			return (path);
		}
		i++;
	}
	return (NULL);
}

char	*path_finder(char *cmd, char **env)
{
	char	**paths;
	int		i;
	int		j;
	char	*path;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	if (!paths)
		return (NULL);
	path = find_executable_path(paths, cmd);
	if (path)
		return (path);
	j = 0;
	while (paths[j])
	{
		free(paths[j]);
		j++;
	}
	free(paths);
	return (NULL);
}

void	check_executable(char *path, struct stat *st, t_mini *mini)
{
	(void)mini;
	if (stat(path, st) == -1)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		ft_exit_gc(127);
	}
	if (S_ISDIR(st->st_mode))
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		ft_exit_gc(126);
	}
	if (!(st->st_mode & S_IXUSR))
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		ft_exit_gc(126);
	}
}

void	exec_command(t_mini *mini, char *path)
{
	execve(path, mini->args, mini->data->env);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": execve error\n", 2);
	ft_exit_gc(126);
}
