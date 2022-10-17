NAME = micro

FLAGS = -Wall -Wextra -Werror

SRC = 	microshell.c \
		micro_utils.c \

LIBFT = Libft/libft.a

OBJ = $(SRC:.c=.o)

CC = gcc -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@rm -rf *.o

lib:
	@make -C ./Libft

clean:
	@rm -rf *.o

fclean: clean
	@rm -rf $(NAME)

re: fclean
	make