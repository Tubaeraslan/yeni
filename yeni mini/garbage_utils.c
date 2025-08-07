/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:15:45 by teraslan          #+#    #+#             */
/*   Updated: 2025/08/07 14:16:42 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*mem_calloc(size_t count, size_t size)
{
	unsigned char	*ptr;
	void			*res;
	size_t			i;

	res = malloc(size * count);
	if (!res)
		return (mem_exit());
	ptr = res;
	i = -1;
	while (++i < size * count)
		ptr[i] = '\0';
	mem_add_new_block(res, size * count);
	return (res);
}

void	*mem_realloc(void *old_data, size_t new_size)
{
	size_t					i;
	void					*res;
	unsigned char			*new_ptr;
	size_t					old_size;
	unsigned char *const	old_ptr = old_data;

	res = malloc(new_size);
	if (!res)
		return (mem_exit());
	i = -1;
	new_ptr = res;
	old_size = mem_find_size(old_data);
	while (++i < (old_size * (old_size < new_size)) + (new_size
			* (new_size < old_size)))
		new_ptr[i] = old_ptr[i];
	mem_add_new_block(res, new_size);
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
