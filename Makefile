# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/31 11:40:29 by horarivo          #+#    #+#              #
#    Updated: 2026/09/03 09:08:21 by horarivo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = codexion

SRC = codexion.c parsing.c init_data.c routine.c utils.c \
      simulation.c dongle.c monitor.c cleanup.c heap.c	 \
	  wakeup.c time.c
OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
