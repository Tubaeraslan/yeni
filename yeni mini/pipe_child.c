/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:28:19 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 20:41:12 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_path_validity(char *path, t_mini *mini)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		print_err_and_exit(path, ": No such file or directory\n", 127, mini);
	if (S_ISDIR(st.st_mode))
		print_err_and_exit(path, ": Is a directory\n", 126, mini);
	if (!(st.st_mode & S_IXUSR))
		print_err_and_exit(path, ": Permission denied\n", 126, mini);
}

static char	*get_command_path(t_mini *mini)
{
	char	*path;

	if (!mini->cmd)
		return (NULL);
	if (mini->cmd[0] == '/' || (mini->cmd[0] == '.' && mini->cmd[1] == '/'))
	{
		path = ft_strdup(mini->cmd);
		if (!path)
		{
			perror("ft_strdup");
			ft_exit_gc(1);
		}
	}
	else
	{
		path = path_finder(mini->cmd, mini->data->env);
		if (!path)
		{
			ft_putstr_fd("command not found: ", 2);
			ft_putstr_fd(mini->cmd, 2);
			ft_putchar_fd('\n', 2);
			ft_exit_gc(127);
		}
	}
	return (path);
}

void	exec_external_or_exit(t_mini *mini)
{
	char	*path;

	path = get_command_path(mini);
	check_path_validity(path, mini);
	execute_commands_pipe(path, mini);
}

static void	handle_child(t_mini *mini, int prev_fd, int *fd)
{
	   int     i;

	   signal(SIGINT, SIG_DFL);
	   signal(SIGQUIT, SIG_DFL);
	   if (mini->parsing_error)
			   ft_exit_gc(1);
	   setup_stdout(mini, fd);
	   setup_stdin(mini, prev_fd); // stdin yönlendirmesi sadece burada yapılacak
	   if (handle_redirections(mini) == -1)
			   ft_exit_gc(1);
	   if (!mini->cmd)
			   ft_exit_gc(0);
	   if (is_built(mini->cmd))
	   {
			   execute_builtin(mini);
			   i = mini->last_exit_code;
			   ft_exit_gc(i);
	   }
	   exec_external_or_exit(mini);
}

pid_t	handle_fork(t_mini *mini, int prev_fd, int *fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		mem_free();
		perror("fork");
		ft_exit_gc(1);
	}
	if (pid == 0)
		handle_child(mini, prev_fd, fd);
	return (pid);
}
