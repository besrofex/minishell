NAME = minishell
LIBFT = libft.a

FLAGS = -Wall -Wextra -Werror
CC = cc 
LIBFT_DIR = libft/

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(LIBFT) $(NAME) 

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(LIBFT_DIR)$(LIBFT) -o $(NAME)

%.o: %.c mini.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	@make clean -C $(LIBFT_DIR)
	rm -f $(OBJ)

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all
.PHONY: clean