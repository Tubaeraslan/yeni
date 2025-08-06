/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:17:18 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/06 14:17:47 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_mini *mini)
{
	int	i;
	int	j;

	i = 0;
	while (mini->data->env[i] != NULL)
	{
		j = 0;
		while (mini->data->env[i][j] != '\0')
		{
			write(1, &mini->data->env[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}