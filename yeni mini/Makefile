NAME = minishell
CFLAGS = -Wall -Wextra -Werror -g
CC = cc
LDFLAGS = -lreadline

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c parse_input.c syntax.c pipe_syntax.c error.c expand.c expand_utils.c \
	heredoc_utils.c heredoc_forks.c heredoc_input.c token.c token_utils.c parsing.c \
	parsing_utils.c mem_utils.c heredoc_check.c execute_commands.c execute_no_pipe.c \
	executable.c built-in.c redirect.c cd.c cd_utils.c env_utils.c pwd.c env.c echo.c \
	export.c export_utils.c unset.c exit.c exit_utils.c execute_with_pipe.c stdin_utils.c \
	pipe_child.c signal.c execute_utils.c init.c garbage_utils.c

OBJ = $(SRC:.c=.o)

all:$(LIBFT) $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS) $(LIBFT)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

leaks: 
	@echo "Running Walgring..."
	@valgrind --leak-check=full		\
			--show-leak-kinds=all	\
			--track-origins=yes		\
			--track-fds=yes			\
			--verbose				\
			--suppressions=valgrind.supp	\
			./$(NAME)

.PHONY: all clean fclean re