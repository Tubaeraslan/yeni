/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_no_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:07:42 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 15:55:59 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_child_process(t_mini *mini)
{
	struct stat	st;
	char		*path;

	if (mini->cmd[0] == '/'
		|| (mini->cmd[0] == '.') || (mini->cmd[0] == '.' && mini->cmd[1] == '.'))
		path = mem_absorb(ft_strdup(mini->cmd));
	else
		path = path_finder(mini->cmd, mini->data->env);
	if (!path)
	{
		ft_putstr_fd(mini->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_exit_gc(127);
	}
	check_executable(path, &st, mini);
	exec_command(mini, path);
}

void	run_child(t_mini *mini)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (handle_redirections(mini) == -1)
		ft_exit_gc(1);
	execute_child_process(mini);
	ft_exit_gc(1);
}
