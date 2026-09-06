/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 10:10:05 by horarivo          #+#    #+#             */
/*   Updated: 2026/09/06 08:33:02 by horarivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_push(t_heap *h, long long key, int coder_id)
{
	int			i;
	int			parent;
	t_wait_node	tmp;

	h->nodes[h->size].key = key;
	h->nodes[h->size].coder_id = coder_id;
	i = h->size;
	h->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!has_priority(h->nodes[i], h->nodes[parent]))
			break ;
		tmp = h->nodes[parent];
		h->nodes[parent] = h->nodes[i];
		h->nodes[i] = tmp;
		i = parent;
	}
}

int	smallest_child(t_heap *h, int i)
{
	int	left;
	int	right;
	int	smallest;

	left = 2 * i + 1;
	right = 2 * i + 2;
	smallest = i;
	if (left < h->size && has_priority(h->nodes[left], h->nodes[smallest]))
		smallest = left;
	if (right < h->size && has_priority(h->nodes[right], h->nodes[smallest]))
		smallest = right;
	return (smallest);
}

int	heap_pop(t_heap *h)
{
	int			min_coder_id;
	int			i;
	int			smallest;
	t_wait_node	tmp;

	min_coder_id = h->nodes[0].coder_id;
	h->size--;
	h->nodes[0] = h->nodes[h->size];
	i = 0;
	while (1)
	{
		smallest = smallest_child(h, i);
		if (smallest == i)
			break ;
		tmp = h->nodes[i];
		h->nodes[i] = h->nodes[smallest];
		h->nodes[smallest] = tmp;
		i = smallest;
	}
	return (min_coder_id);
}

int	heap_peek(t_heap *h)
{
	if (h->size == 0)
		return (-1);
	return (h->nodes[0].coder_id);
}

void	heap_remove(t_heap *h, int coder_id)
{
	int			i;
	int			smallest;
	t_wait_node	tmp;

	i = 0;
	while (i < h->size && h->nodes[i].coder_id != coder_id)
		i++;
	if (i >= h->size)
		return ;
	h->size--;
	h->nodes[i] = h->nodes[h->size];
	while (1)
	{
		smallest = smallest_child(h, i);
		if (smallest == i)
			break ;
		tmp = h->nodes[i];
		h->nodes[i] = h->nodes[smallest];
		h->nodes[smallest] = tmp;
		i = smallest;
	}
}
