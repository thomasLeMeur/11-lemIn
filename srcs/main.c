/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 17:20:35 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/07 14:25:10 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	free_and_quit_lists(t_list *lists[2], int ret)
{
	ft_lstdel(&lists[0], &my_lstdel);
	ft_lstdel(&lists[1], &my_lstdel);
	if (ret == 0)
		ft_putendl("\033[31mERROR\033[0m");
	return (ret);
}

static int	free_and_quit(t_lemin *l, int ret, int is_finish, int opt)
{
	int	i;

	i = -1;
	if (is_finish && opt)
	{
		if (l->nb_turns < 2)
			ft_printf("\033[1m\033[4mIt tooks %d turn.\n\033[0m",
			l->nb_turns);
		else
			ft_printf("\033[1m\033[4mIt tooks %d turns.\n\033[0m",
			l->nb_turns);
	}
	if (l->r)
		while (++i < l->nb_rooms)
		{
			ft_memdel((void **)&l->r[i].r);
			ft_memdel((void **)&l->r[i].w);
			ft_memdel((void **)&l->r[i].n);
		}
	ft_memdel((void **)&l->a);
	ft_memdel((void **)&l->r);
	ft_memdel((void **)&l);
	return (ret);
}

static int	my_stop_cond(t_lemin *l)
{
	int	i;

	i = -1;
	while (++i < l->nb_ants)
		if (l->a[i] != l->r[l->nb_rooms - 1].id)
			return (0);
	return (1);
}

static void	compute(t_lemin *l)
{
	int	i;
	int	j;

	while (!my_stop_cond(l) && (i = -1))
	{
		while (++i < l->nb_ants && l->a[i])
		{
			if (l->a[i] == l->r[l->nb_rooms - 1].id)
				continue ;
			ft_printf("\033[33mL%d-%s ",
			i + 1, l->r[(l->a[i] = *l->r[l->a[i]].n)].r);
		}
		j = 0;
		while (++i && j < l->r[0].nb_nexts && l->r[l->r[0].n[j]].send-- > 0)
			ft_printf("\033[33mL%d-%s ",
			i, l->r[(l->a[i - 1] = l->r[0].n[j++])].r);
		ft_putendl("\033[0m");
		l->nb_turns++;
	}
}

int			main(int ac, char **av)
{
	t_lemin	*l;
	t_list	*lists[2];

	lists[0] = NULL;
	lists[1] = NULL;
	if (!(l = (t_lemin *)ft_memalloc(sizeof(t_lemin))))
		return (0);
	if (!my_parse(l, lists, NULL))
	{
		ft_putchar('\n');
		return (free_and_quit_lists(lists, free_and_quit(l, 0, 0, 0)));
	}
	ft_putchar('\n');
	if (!infos_to_struct(l, lists))
		return (free_and_quit_lists(lists, free_and_quit(l, 0, 0, 0)));
	free_and_quit_lists(lists, 1);
	compute(l);
	return (free_and_quit(l, 0, 1,
	(ac > 1 && av[1][0] == '-' && av[1][1] == 't')));
}
