/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 17:20:35 by tle-meur          #+#    #+#             */
/*   Updated: 2016/02/18 15:39:01 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	find_id(t_room *rooms, char *name, int nb_rooms)
{
	int	i;

	i = -1;
	while (++i < nb_rooms)
		if (!ft_strcmp(rooms[i].r, name))
			return (i);
	return (-1);
}

static int	my_update(t_list **tmp, t_list **tubes)
{
	t_list	*next;

	if (*tmp == *tubes)
		*tubes = (*tmp)->next;
	else
	{
		next = *tubes;
		while (next->next != *tmp)
			next = next->next;
		next->next = (*tmp)->next;
	}
	ft_lstdelone(tmp, &my_lstdel);
	return (1);
}

static int	treat_one_room_3(t_lemin *l, int v[4], t_room *prev, t_room *r1)
{
	t_room	*r2;

	r2 = &l->r[v[1]];
	r2->nb_ways = r1->nb_ways + ((++v[2] == 1) ? 0 : 1);
	if (!(r2->w = (int *)malloc(r2->nb_ways * sizeof(int))))
		return (0);
	ft_memcpy(r2->w, r1->w, r1->nb_ways * sizeof(int));
	if (v[2] > 1 && ((r2->w[r2->nb_ways - 1] = l->id_way) || 1))
		if (v[2] == 2)
		{
			ft_memdel((void **)&prev->w);
			prev->nb_ways = r1->nb_ways + 1;
			if (!(prev->w = (int *)malloc(prev->nb_ways * sizeof(int))))
				return (0);
			ft_memcpy(prev->w, r1->w, r1->nb_ways * sizeof(int));
			prev->w[prev->nb_ways - 1] = l->id_way;
		}
	if (!r2->n)
		if (!(r2->n = (int *)malloc(++r2->nb_nexts * sizeof(int))))
			return (0);
	return (1 + 0 * (*r2->n = *v));
}

static int	treat_one_room_2(t_lemin *l, int v[4], t_room *prev)
{
	int		i;
	int		j;
	t_room	*r1;
	t_room	*r2;
	int		*buf;

	r1 = &l->r[*v];
	r2 = &l->r[v[1]];
	if ((i = -1) && v[1])
		return (treat_one_room_3(l, v, prev, r1));
	while (++i < r2->nb_ways && (j = -1))
		while (++j < r1->nb_ways)
			if (r1->w[j] > 0 && r1->w[j] == r2->w[i])
				return (2);
	if (!(buf = (int *)ft_memrealloc(r2->w, r2->nb_ways * sizeof(int),
		(r2->nb_ways + r1->nb_ways) * sizeof(int))))
		return (0);
	ft_memcpy((r2->w = buf) + r2->nb_ways, r1->w, r1->nb_ways * sizeof(int));
	r2->nb_ways += r1->nb_ways;
	if (!(buf = (int *)ft_memrealloc(r2->n, r2->nb_nexts * sizeof(int),
		(r2->nb_nexts + 1) * sizeof(int))))
		return (0);
	r2->n = buf;
	return (2 + 0 * (r2->n[r2->nb_nexts++] = *v));
}

int			treat_one_room(t_lemin *l, int id1, t_list **tubes,
			t_list **list)
{
	int		v[4];
	t_list	*tmp;
	t_room	*prev;

	*v = id1;
	v[2] = 0;
	while ((tmp = ft_lstfind(*tubes, l->r[*v].r, sizeof(t_tube),
			&my_lstcmp2)))
	{
		v[1] = find_id(l->r, (ft_strcmp(l->r[*v].r,
			((t_tube *)tmp->content)->room1)) ? ((t_tube *)tmp->content)->room1
			: ((t_tube *)tmp->content)->room2, l->nb_rooms);
		if (my_update(&tmp, tubes) && v[1] && l->r[*v].far + 2 > l->r[v[1]].far)
			continue ;
		l->r[v[1]].far = l->r[*v].far + 1;
		if (!(v[3] = treat_one_room_2(l, v, prev))
			|| (v[3] == 1 && !(tmp = ft_lstnew(&v[1], sizeof(int)))))
			return (0);
		else if (v[3] == 1)
			ft_lstpush(list, tmp);
		prev = (v[2] == 1) ? &l->r[v[1]] : prev;
	}
	return ((l->id_way += (v[2] > 1) ? 1 : 0) + 1);
}
