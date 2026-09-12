# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: horarivo <horarivo@student.42antananari    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/31 11:40:29 by horarivo          #+#    #+#              #
#    Updated: 2026/09/12 23:16:33 by horarivo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = codexion
 
SRC_DIR = src
OBJ_DIR = objs
 
SRC = codexion.c parsing.c init_data.c routine.c utils.c \
      simulation.c dongle.c monitor.c cleanup.c heap.c \
      wakeup.c time.c
 
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
 
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -I $(SRC_DIR)
 
all: $(NAME)
 
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/codexion.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
 
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
 
clean:
	rm -rf $(OBJ_DIR)
 
fclean: clean
	rm -f $(NAME)
 
re: fclean all
 
.PHONY: all clean fclean re
 
