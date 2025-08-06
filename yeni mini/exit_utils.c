/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:09:40 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 15:10:06 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_control(t_mini	*mini)
{
	if (mini->args[1] && mini->args[2] && numeric_control(mini->args[1]) == 1)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(mini->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit_program(mini, 2);
	}
}

void	exit_control2(t_mini	*mini)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(mini->args[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit_program(mini, 2);
}
