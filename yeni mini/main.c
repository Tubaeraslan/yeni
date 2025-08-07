/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:46:18 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 13:11:23 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_status = 0;

void	ft_exit_gc(int status)
{
	mem_free();
	exit(status);
}

char	**ft_env_dup(char **envp)
{
	int		i;
	char	**env_copy;

	i = 0;
	while (envp && envp[i])
		i++;
	env_copy = mem_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp && envp[i])
	{
		env_copy[i] = mem_absorb(ft_strdup(envp[i]));
		if (!env_copy[i])
		{
			return (NULL);
		}
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

static int	read_and_prepare_input(t_mini *mini)
{
	char	*input;

	input = readline("minishell$ ");
	if (!input)
	{
		printf("exit\n");
		return (1);
	}
	add_history(input);
	mini->input = mem_absorb(ft_strdup(input));
	free(input);
	return (0);
}

static void	minishell_loop(t_mini *mini)
{
	while (1)
	{
		g_signal_status = 0;
		reset_mini_for_new_command(mini);
		if (read_and_prepare_input(mini))
			break ;
		if (g_signal_status != 0)
		{
			mini->last_exit_code = g_signal_status;
			g_signal_status = 0;
		}
		parse_input(mini);
		execute_commands(mini);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	*mini;
	t_data	*data;

	(void)argc;
	(void)argv;
	mini = mem_malloc(sizeof(t_mini));
	data = mem_malloc(sizeof(t_data));
	init_minishell(mini, data, envp);
	handle_signals();
	minishell_loop(mini);
	ft_close_fds(mini);
	mem_free();
	return (0);
}
