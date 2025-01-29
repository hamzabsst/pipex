CC = cc
CFLAGS = -Wall -Werror -Wextra

MYLIB_DIR = mylib
MYLIB = $(MYLIB_DIR)/myLib.a

NAME = pipex

SRCS =	src/main.c \
		src/pipex.c \
		src/get_commands.c \

OBJS = $(SRCS:.c=.o)

all: $(MYLIB) $(NAME)

$(MYLIB):
		@$(MAKE) -C $(MYLIB_DIR)

%.o: %.c inc/pipex.h
		@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(MYLIB)
		@$(CC) $(CFLAGS) $(OBJS) $(MYLIB) -o $(NAME)

clean:
		@rm -f $(OBJS)
		@$(MAKE) clean -C $(MYLIB_DIR)

fclean: clean
		@rm -f $(NAME)
		@$(MAKE) fclean -C $(MYLIB_DIR)

re: fclean all

.PHONY: all clean fclean re
