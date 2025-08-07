/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:19:37 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/07 15:23:15 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*mem_exit(void)
{
	const char	*err = "libmem: Out of memory\n";
	int			i;

	i = -1;
	while (err[++i])
		write(STDERR_FILENO, &err[i], 1);
	mem_free();
	exit(EXIT_FAILURE);
	return (NULL);
}

t_mem_block	*mem_add_new_block(void *data, size_t size)
{
	static t_mem_block	*head;
	t_mem_block			*new;
	t_mem_block			*node;

	new = malloc(sizeof(t_mem_block));
	if (!new)
		return (mem_exit());
	new->data = data;
	new->size = size;
	new->head = &head;
	new->next = NULL;
	if (!head)
		head = new;
	else
	{
		node = head;
		while (node->next)
			node = node->next;
		node->next = new;
	}
	return (new);
}

void	mem_clear_block(t_mem_block **head)
{
	t_mem_block	*temp;
	t_mem_block	*node;

	if (!(*head))
		return ;
	node = *head;
	while (node)
	{
		temp = node->next;
		if (node->data)
		{
			free(node->data);
			node->data = NULL;
		}
		free(node);
		node = temp;
	}
	*head = NULL;
}

size_t	mem_find_size(void *data)
{
	t_mem_block	*node;
	size_t		res;

	node = *(mem_add_new_block(NULL, 0)->head);
	while (1)
	{
		if (node->data == data)
			res = node->size;
		if (node->next)
			node = node->next;
		else
			break ;
	}
	return (res);
}

void	*mem_malloc(size_t size)
{
	void	*res;

	res = malloc(size);
	if (!res)
		return (mem_exit());
	mem_add_new_block(res, size);
	return (res);
}
