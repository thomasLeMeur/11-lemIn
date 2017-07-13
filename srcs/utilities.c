/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 17:20:35 by tle-meur          #+#    #+#             */
/*   Updated: 2016/02/18 12:44:11 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	my_lstdel(void *content, size_t size)
{
	if (size == sizeof(t_line))
		ft_memdel((void **)&((t_line *)content)->part1);
	else if (size == sizeof(t_room))
		ft_memdel((void **)&((t_room *)content)->r);
	else if (size == sizeof(t_tube))
	{
		ft_memdel((void **)&((t_tube *)content)->room1);
		ft_memdel((void **)&((t_tube *)content)->room2);
	}
	ft_memdel(&content);
}

int		my_lstcmp(const t_list *elem, const void *ref, size_t size)
{
	char	*str;
	char	*tmp;

	if (size == sizeof(t_room))
		return (!ft_strcmp(((t_room *)elem->content)->r, ref));
	else
	{
		if (!(str = ft_strjoin(NULL, ((t_tube *)elem->content)->room1)))
			return (0);
		tmp = str;
		if (!(str = ft_strjoin(tmp, "-")))
		{
			ft_memdel((void **)&tmp);
			return (0);
		}
		free(tmp);
		tmp = str;
		if (!(str = ft_strjoin(tmp, ((t_tube *)elem->content)->room2)))
		{
			ft_memdel((void **)&tmp);
			return (0);
		}
		return (free_and_quit_chars(&tmp, &str, NULL, !ft_strcmp(str, ref)));
	}
}

int		my_lstcmp2(const t_list *elem, const void *ref, size_t size)
{
	(void)size;
	return (!ft_strcmp(ref, ((t_tube *)elem->content)->room1)
		|| !ft_strcmp(ref, ((t_tube *)elem->content)->room2));
}

int		free_and_quit_chars(char **s1, char **s2, char ***s3, int ret)
{
	int	i;

	i = -1;
	ft_memdel((void **)s1);
	ft_memdel((void **)s2);
	if (s3 && *s3)
	{
		while ((*s3)[++i])
			free((*s3)[i]);
		ft_memdel((void **)s3);
	}
	return (ret);
}

char	*my_join(char **s, int limit, char *link)
{
	int		i;
	char	*str;
	char	*tmp;

	i = 0;
	tmp = NULL;
	str = NULL;
	while (i <= limit && s[i])
	{
		str = ft_strjoin(tmp, s[i]);
		ft_memdel((void **)&tmp);
		if (!(tmp = str))
			break ;
		if (i != limit && s[i + 1])
		{
			str = ft_strjoin(tmp, link);
			ft_memdel((void **)&tmp);
			if (!(tmp = str))
				break ;
		}
		i++;
	}
	return (str);
}
