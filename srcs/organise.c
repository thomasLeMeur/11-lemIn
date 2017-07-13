/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 17:20:35 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/16 13:55:40 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "lemin.h"

static int	init_tubes_and_way(t_lemin *l, t_list **tubes)
{
	int		id;
	int		ret;
	t_list	*list;

	list = NULL;
	if (!(ret = treat_one_room(l, l->nb_rooms - 1, tubes, &list)))
		while (list)
			ft_lstdequeue(&list);
	if (!list && !l->r[0].n)
		return (0);
	while (list)
	{
		id = *(int *)list->content;
		ft_lstdequeue(&list);
		if (!treat_one_room(l, id, tubes, &list))
		{
			while (list)
				ft_lstdequeue(&list);
			return (0);
		}
	}
	return (1);
}

static int	first_init_rooms(t_lemin *l, t_list *rooms)
{
	int		i;
	t_room	*room;

	l->nb_rooms = ft_lstsize(rooms);
	if (!(l->r = (t_room *)ft_memalloc(l->nb_rooms * sizeof(t_room))))
		return (0);
	i = -1;
	while (++i < l->nb_rooms && (room = &l->r[i]))
	{
		room->id = i;
		room->far = INT_MAX;
		room->posx = ((t_room *)rooms->content)->posx;
		room->posy = ((t_room *)rooms->content)->posy;
		if (!(room->r = ft_strdup(((t_room *)rooms->content)->r)))
			return (0);
		rooms = rooms->next;
	}
	return (1);
}

static int	compute_better_distrib(t_lemin *l)
{
	int	i;
	int	id;
	int	nbs[2];
	int	sum;
	int	tmp[l->r[0].nb_nexts];

	nbs[1] = l->nb_ants;
	id = l->r[0].nb_nexts;
	while (nbs[1] && --id >= 0 && (i = -1))
	{
		sum = 0;
		*nbs = 1 + l->r[l->r[0].n[id]].far;
		while (++i <= id)
			sum += (tmp[i] = *nbs - l->r[l->r[0].n[i]].far);
		if ((tmp[id] = 1) && (*nbs -= *nbs - sum) && nbs[1] / sum == 0)
			continue ;
		while (*nbs + id + 1 <= nbs[1] && tmp[id]++)
			*nbs += id + 1;
		nbs[1] -= (sum += (tmp[id] - 1) * (id + 1) + (i = -1) + 1);
		while (++i < id)
			l->r[l->r[0].n[i]].send += tmp[i] + tmp[id] - 1;
		l->r[l->r[0].n[id]].send += tmp[id];
	}
	return (1);
}

int			infos_to_struct(t_lemin *l, t_list *lists[2])
{
	t_list	*tmp;
	t_list	*next;

	if (!(next = lists[0]) || l->start_end != 2)
		return (0);
	while ((tmp = next)
		&& (next = tmp->next))
		if (((t_room *)next->content)->is_end && ((tmp->next = next->next) + 1))
		{
			tmp = next;
			tmp->next = NULL;
			ft_lstpush(&lists[0], tmp);
			break ;
		}
	if (!first_init_rooms(l, lists[0]))
		return (0);
	if (!(l->a = ft_memalloc(l->nb_ants * sizeof(int))))
		return (0);
	l->r[l->nb_rooms - 1].far = 0;
	if (!(l->r[l->nb_rooms - 1].w = (int *)malloc(sizeof(int))))
		return (0);
	l->r[l->nb_rooms - 1].w[l->r[l->nb_rooms - 1].nb_ways++] = -1;
	if (!init_tubes_and_way(l, &lists[1]))
		return (0);
	return (compute_better_distrib(l));
}
