CC = cc
CFLAGS = -Wall -Werror -Wextra

MYLIB_DIR = mylib
MYLIB = $(MYLIB_DIR)/myLib.a

NAME = pipex

SRCS =	src/main.c \
		src/pipex.c \
		src/get_commands.c \

SRCS_BONUS =	bonus/pipex_bonus.c \
				bonus/utils_bonus.c \

OBJS = $(SRCS:.c=.o)

OBJS_BONUS = $(SRCS_BONUS:.c=.o)

all: $(MYLIB) $(NAME)

$(MYLIB):
		@$(MAKE) -C $(MYLIB_DIR)

%.o: %.c inc/pipex.h
		@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(MYLIB)
		@$(CC) $(CFLAGS) $(OBJS) $(MYLIB) -o $(NAME)

OBJS_NO_MAIN = $(filter-out src/main.o, $(OBJS))

$(NAME_BONUS): $(OBJS_NO_MAIN) $(MYLIB) $(OBJS_BONUS)
			@$(CC) $(CFLAGS) $(OBJS_NO_MAIN) $(OBJS_BONUS) $(MYLIB) -o $(NAME)

bonus: $(NAME_BONUS)

clean:
		@rm -f $(OBJS) $(OBJS_BONUS)
		@$(MAKE) clean -C $(MYLIB_DIR)

fclean: clean
		@rm -f $(NAME) $(NAME_BONUS)
		@$(MAKE) fclean -C $(MYLIB_DIR)

re: fclean all

.PHONY: all clean fclean re bonus
