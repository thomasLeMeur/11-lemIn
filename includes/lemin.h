/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 17:14:24 by tle-meur          #+#    #+#             */
/*   Updated: 2016/02/18 15:37:06 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# include "../libft/libft.h"

# define ROOM_TYPE	1
# define TUBE_TYPE	2
# define NB_TYPE	3

typedef struct	s_line
{
	char	type;
	int		part2;
	int		part3;
	char	*part1;
	char	is_end;
	char	is_start;
}				t_line;

typedef struct	s_way
{
	int	id;
	int	*ways;
	int	nb_ways;
}				t_way;

typedef struct	s_room
{
	int		*n;
	char	*r;
	int		*w;
	int		id;
	int		far;
	int		posx;
	int		posy;
	int		send;
	int		nb_ways;
	int		nb_nexts;
	int		is_end;
}				t_room;

typedef struct	s_tube
{
	char	*room1;
	char	*room2;
}				t_tube;

typedef struct	s_lemin
{
	int		*a;
	t_room	*r;
	int		id_way;
	int		nb_ants;
	int		nb_rooms;
	int		nb_turns;
	char	start_end;
}				t_lemin;

int				infos_to_struct(t_lemin *l, t_list *lists[2]);
int				my_parse(t_lemin *l, t_list *lists[2], char *line);
int				my_add_lists(t_lemin *l, t_list *lists[2], t_line *inf);

void			my_lstdel(void *content, size_t size);
char			*my_join(char **s, int limit, char *link);
int				my_lstcmp(const t_list *elem, const void *ref, size_t size);
int				my_lstcmp2(const t_list *elem, const void *ref, size_t size);
int				free_and_quit_chars(char **s1, char **s2, char ***s3, int ret);
int				treat_one_room(t_lemin *l, int id1, t_list **tubes,
				t_list **list);

#endif
