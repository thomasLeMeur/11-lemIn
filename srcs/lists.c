/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 17:20:35 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/11 18:17:00 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "lemin.h"

static int	add_tube(t_list *lists[2], t_line *inf)
{
	int		i;
	char	**s;
	t_tube	tmp;
	t_list	*add;

	ft_memset(&tmp, (i = -1) + 1, sizeof(t_tube));
	if (!(s = ft_strsplit(inf->part1, '-')))
		return (0);
	while (s[++i + 1])
	{
		if (!(tmp.room1 = my_join(s, i, "-"))
			|| !(tmp.room2 = my_join(s + i + 1, INT_MAX, "-")))
			return (free_and_quit_chars(&tmp.room1, &tmp.room2, &s, 0));
		if (!ft_lstfind(lists[0], tmp.room1, sizeof(t_room), &my_lstcmp)
			|| !ft_lstfind(lists[0], tmp.room2, sizeof(t_room), &my_lstcmp))
			return (free_and_quit_chars(&tmp.room1, &tmp.room2, &s, -1));
		if (!(add = ft_lstnew(&tmp, sizeof(t_tube))))
			return (free_and_quit_chars(&tmp.room1, &tmp.room2, &s, 0));
		if (!(((t_tube*)add->content)->room1 = ft_strdup(tmp.room1)) ||
			!(((t_tube*)add->content)->room2 = ft_strdup(tmp.room2)))
			return (free_and_quit_chars(&tmp.room1, &tmp.room2, &s, 0));
		ft_lstadd(&lists[1], add);
		return (free_and_quit_chars(&tmp.room1, &tmp.room2, &s, 1));
	}
	return (1);
}

static int	add_room(t_lemin *l, t_list *lists[2], t_line *inf)
{
	t_room	tmp;
	t_list	*add;

	if ((add = ft_lstfind(lists[0], inf->part1, sizeof(t_room), &my_lstcmp)))
		return (-1);
	ft_memset(&tmp, 0, sizeof(t_room));
	if (!(tmp.r = ft_strdup(inf->part1)))
		return (0);
	tmp.posx = inf->part2;
	tmp.posy = inf->part3;
	if (inf->is_end)
	{
		l->start_end++;
		tmp.is_end = 1;
	}
	if (!(add = ft_lstnew(&tmp, sizeof(t_room))))
		return (0);
	if (inf->is_start)
	{
		l->start_end++;
		ft_lstadd(&lists[0], add);
	}
	else
		ft_lstpush(&lists[0], add);
	return (1);
}

int			my_add_lists(t_lemin *l, t_list *lists[2], t_line *inf)
{
	if (inf->type == NB_TYPE)
		return (((l->nb_ants = ft_atoi(inf->part1)) <= 0) ? 0 : 1);
	if (inf->type == ROOM_TYPE)
		return (add_room(l, lists, inf));
	if (inf->type == TUBE_TYPE)
		return (add_tube(lists, inf));
	return (0);
}
