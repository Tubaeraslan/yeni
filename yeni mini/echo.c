/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:19:20 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 14:55:02 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || !arg[1])
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(t_mini *mini)
{
    int	i;
    int	newline;
    int	printed_any;

    i = 1;
    newline = 1;
    while (mini->args[i] && is_n_flag(mini->args[i]))
    {
        newline = 0;
        i++;
    }
    printed_any = 0;
    while (mini->args[i])
    {
        if (printed_any)
            ft_putchar_fd(' ', 1);
        if (*(mini->args[i]) != '\0' || printed_any || mini->args[i + 1])
        {
            ft_putstr_fd(mini->args[i], 1);
            printed_any = 1;
        }
        i++;
    }
    if (newline)
        ft_putchar_fd('\n', 1);
    mini->last_exit_code = 0;
}
