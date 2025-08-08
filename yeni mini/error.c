/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:39:45 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 15:10:13 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_error(t_mini *mini, const char *msg)
{
	printf("%s\n", msg);
	mini->parsing_error = 1;
	mini->last_exit_code = 2;
}

void	handle_fork_error(t_mini *mini)
{
	(void)mini;
	perror("fork");
	ft_exit_gc(1);
}

void	print_err_and_exit(char *path, char *msg, int code, t_mini *mini)
{
	(void)mini;
	ft_putstr_fd(path, 2);
	ft_putstr_fd(msg, 2);
	ft_exit_gc(code);
}
