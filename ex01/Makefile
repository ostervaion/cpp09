# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juetxeba <juetxeba@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/04 15:52:58 by juetxeba          #+#    #+#              #
#    Updated: 2025/09/05 16:43:35 by juetxeba         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= RPN
FLAGS	= -Wall -Wextra -Werror -std=c++98 -I Include -ggdb
SRC_DIR = srcs/
SRCS	= $(SRC_DIR)main.cpp
OBJS	= $(SRCS:.cpp=.o)
CC		= c++

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(FLAGS) -o $(NAME)
	@echo "Executable $(NAME) created!"

%.o: %.cpp 
	@echo "Compiling $< -> $@"
	@$(CC) -c $(FLAGS) $< -o $@

clean:
	@rm -f $(OBJS)
	@echo "Object files cleaned!"

fclean: clean
	@rm -f $(NAME)
	@echo "Executable cleaned!"

start:
	@mkdir -p Include
	@mkdir -p srcs
	@echo "Directories created"

moveS:
	@mv *.c* srcs

moveI:
	@mv *.h* Include

redistribute: start moveI moveS

re: fclean all
.PHONY: all clean fclean re
