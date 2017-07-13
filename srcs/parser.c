/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 17:20:35 by tle-meur          #+#    #+#             */
/*   Updated: 2016/03/07 16:03:58 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	my_isnb(char *s)
{
	if (*s == '-' || *s == '+')
		s++;
	while (*s)
		if (!ft_isdigit(*(s++)))
			return (0);
	return (1);
}

static int	init_infos(t_line *infos, char *s)
{
	infos->type = 0;
	if (!(infos->part1 = ft_strdup(s)))
		return (0);
	infos->part2 = -1;
	infos->part3 = -1;
	infos->is_end = 0;
	infos->is_start = 0;
	return (1);
}

static int	continue_continue_parse(t_line *infos, char **s, int v[3], int r)
{
	int	len;

	if (!v[2] && (infos->type = NB_TYPE))
	{
		if (!(v[2] = 1) || r != 1 || !my_isnb(*s))
			return (0);
	}
	else if (r < 3 || (v[2] == 1 && (!my_isnb(s[r - 2]) || !my_isnb(s[r - 1]))))
		v[2] = 2;
	else if (v[2] == 1 && (infos->type = ROOM_TYPE))
	{
		len = ft_strlen(infos->part1) - 1;
		while (v[2]-- >= 0)
			while (!(infos->part1[len] *= 0) && infos->part1[--len] != ' ')
				;
		infos->part2 = ft_atoi(s[r - 2]) + (infos->part1[len] *= 0);
		if ((v[2] += 3) && ((infos->part3 = ft_atoi(s[r - 1])) || 1) && *v & 1)
			infos->is_start = (v[1] == 1) ? (v[1] *= 0) : (*v -= *v - 4);
		else if (v[1] & 1)
			infos->is_end = (*v == 1) ? (*v *= 0) : (v[1] -= v[1] - 4);
		else if ((*v && *v != 4) || (v[1] && v[1] != 4))
			return (0);
	}
	return ((v[2] == 2 && (infos->type = TUBE_TYPE)
		&& !ft_strchr(infos->part1, '-')) ? 0 : 1);
}

static int	continue_parse(t_lemin *l, t_list *lists[2], char **s, int v[5])
{
	int		ret;
	t_line	infos;
	char	*part1;

	if (v[3] && v[4] == 1 && !ft_strcmp("start", *s))
		return ((*v = (*v < 2) ? 1 : *v + 2) + (v[1] *= (v[1] == 1) ? 0 : 1));
	if (v[3] && v[4] == 1 && !ft_strcmp("end", *s))
		return ((v[1] = (v[1] < 2) ? 1 : v[1] + 2) + (*v *= (*v == 1) ? 0 : 1));
	else if (v[3])
		return (1);
	if (!(part1 = my_join(s, v[4] - 1, " ")))
		return (0);
	if (!init_infos(&infos, part1))
		return (free_and_quit_chars(&infos.part1, &part1, NULL, 0));
	if (!continue_continue_parse(&infos, s, v, v[4]))
		return (free_and_quit_chars(&infos.part1, &part1, NULL, -1));
	if ((ret = my_add_lists(l, lists, &infos)) < 1)
		return (free_and_quit_chars(&infos.part1, &part1, NULL, ret));
	return (free_and_quit_chars(&infos.part1, &part1, NULL, 1));
}

int			my_parse(t_lemin *l, t_list *lists[2], char *line)
{
	char	**s;
	char	*tmp;
	int		vars[5];

	s = NULL;
	ft_memset(vars, 0, 5 * sizeof(int));
	while ((vars[4] = ft_gnl(0, &line)) > 0
			&& (tmp = line) && !(vars[3] *= 0))
		if (!*tmp || *tmp == 'L')
			return (free_and_quit_chars(&line, NULL, NULL, 1));
		else if ((vars[4] = -1) && *tmp == '#' && *(++tmp) != '#')
			ft_printf("\033[32m%s\n\033[0m", line);
		else if (!(s = ft_strsplit((*tmp == '#' && (vars[3] += 1))
			? ++tmp : tmp, ' ')))
			return (free_and_quit_chars(&line, NULL, NULL, 0));
		else
		{
			while (s[++vars[4]])
				;
			if ((vars[4] = continue_parse(l, lists, s, vars)) < 1)
				return (free_and_quit_chars(&line, NULL, &s, vars[4]));
			free_and_quit_chars(NULL, NULL, &s, 0);
			ft_printf("\033[32m%s\n\033[0m", line);
		}
	return (free_and_quit_chars(&line, NULL, &s, vars[4] + 1));
}
