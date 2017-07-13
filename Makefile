# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/01/14 15:29:40 by tle-meur          #+#    #+#              #
#    Updated: 2016/03/07 14:41:37 by tle-meur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	lem-in

FLAGS	=	-g -Wall -Wextra -Werror

SRCS	=	srcs/main.c srcs/parser.c srcs/lists.c srcs/organise.c \
			srcs/treat_room.c srcs/utilities.c

OBJS	=	$(SRCS:.c=.o)

all		:	$(NAME)

%.o		:	%.c
			@gcc $(FLAGS) -o $@ -c $< -I includes/

$(NAME)	:	$(OBJS)
			@make -C libft/ simple
			@echo "libft.a created"
			@gcc $(FLAGS) -o $@ $^ -L libft/ -lft
			@echo "$(NAME) created"
clean	:
			@rm -f $(OBJS)
			@make -C libft/ clean
			@echo "objects deleted"

fclean	:	clean
			@make -C libft/ fclean
			@rm -f $(NAME)
			@echo "$(NAME) libft.a deleted"

re		:	fclean $(NAME)

simple	:	re
			@rm -f $(OBJS)
			@make -C libft/ clean
			@echo "objects deleted"

.PHONY	:	all clean fclean re simple
