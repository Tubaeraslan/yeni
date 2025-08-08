/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:15:45 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/08 18:59:54 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*mem_malloc(size_t size)
{
	void	*res;

	res = malloc(size);
	if (!res)
		return (mem_exit());
	mem_add_new_block(res, size);
	return (res);
}

void	mem_free(void)
{
	mem_clear_block(mem_add_new_block(NULL, 0)->head);
}

void	*mem_absorb(void *addr)
{
	mem_add_new_block(addr, 1);
	return (addr);
}
