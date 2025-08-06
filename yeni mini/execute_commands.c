/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:52:39 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 18:40:01 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands_pipe(char *path, t_mini *mini)
{
	execve(path, mini->args, mini->data->env);
	perror("execve");
	ft_exit_gc(126);
}

static void	set_exit_code(t_mini *mini, int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\r", 1);
			mini->last_exit_code = 130;
		}
		else if (sig == SIGQUIT)
		{
			write(1, "Quit (core dumped)\n", 19);
			mini->last_exit_code = 131;
		}
		else
			mini->last_exit_code = 128 + sig;
	}
	else if (WIFEXITED(status))
		mini->last_exit_code = WEXITSTATUS(status);
}

void execute_a_command(t_mini *mini)
{
	pid_t pid;
	int status;

	if (!mini || !mini->cmd)
		return ;
	if (is_built(mini->cmd))
	{
		execute_builtin(mini);
		return;
	}
	pid = fork();
	if (pid == -1)
		handle_fork_error(mini);
	else if (pid == 0)
		run_child(mini);
	else
	{
		waitpid(pid, &status, 0);
		set_exit_code(mini, status);	
	}
}

int	execute_many_commands(t_mini *mini)
{
	int	cmd_count;
	int	exit_code;
	//int	validation_result;

	cmd_count = count_commands(mini);
	mini->pids = mem_malloc(sizeof(pid_t) * cmd_count);
	if (!mini->pids)
	{
		mem_free();
		return (perror("malloc"), EXIT_FAILURE);
	}
	exit_code = execute_pipeline(mini, mini->pids);
	// if (mini->pids)
	// {	
	// 	free(mini->pids);
	// 	mini->pids = NULL;
	// }
	if (exit_code == -1)
		return (EXIT_FAILURE);
	else
		return (exit_code);
}

void execute_commands(t_mini *mini)
{
	t_mini	*current;

	current = mini;
	if (!mini->is_pipe)
	{
		if (!mini->parsing_error)
			execute_a_command(current);
	}
	else
	{
		// For pipes, always execute - individual command errors are handled per command
		mini->last_exit_code = execute_many_commands(mini);
	}
}
