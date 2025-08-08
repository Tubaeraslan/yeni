/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:54:55 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 12:33:18 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid(char *str)
{
	int	i;

	i = 0;
	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	execute_b(t_mini *mini)
{
	char	*cmd_name;
	int		len;

	if (!mini || !mini->cmd)
		return ;
	cmd_name = mini->cmd;
	len = ft_strlen(mini->cmd);
	while (len > 0 && mini->cmd[len] != '/')
		len--;
	if (mini->cmd[len] == '/')
		cmd_name = &mini->cmd[len + 1];
	if (ft_strncmp(cmd_name, "echo", 5) == 0)
		ft_echo(mini);
	else if (ft_strncmp(cmd_name, "pwd", 4) == 0)
		ft_pwd(mini);
	else if (ft_strncmp(cmd_name, "env", 4) == 0)
		ft_env(mini);
	else if (ft_strncmp(cmd_name, "exit", 5) == 0)
		mini->last_exit_code = ft_exit(mini);
	else if (ft_strncmp(cmd_name, "export", 7) == 0)
		ft_export(mini);
	else if (ft_strncmp(cmd_name, "unset", 6) == 0)
		ft_unset(mini);
	else if (ft_strncmp(cmd_name, "cd", 3) == 0)
		ft_cd(mini);
}

int	is_built(char *arg)
{
	char	*cmd;
	int		len;

	if (!arg)
		return (0);
	cmd = arg;
	len = ft_strlen(arg);
	while (len > 0 && arg[len] != '/')
		len--;
	if (arg[len] == '/')
		cmd = &arg[len + 1];
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

static int	execute_builtin_redir(t_mini *mini, int saved_in, int saved_out)
{
	if (handle_redirections(mini) == -1)
	{
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		mini->last_exit_code = 1;
		return (1);
	}
	if (ft_strncmp(mini->cmd, "exit", 5) == 0)
	{
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		mini->last_exit_code = ft_exit(mini);
		return (1);
	}
	return (0);
}

void	execute_builtin(t_mini *mini)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (execute_builtin_redir(mini, saved_stdin, saved_stdout))
		return ;
	execute_b(mini);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
