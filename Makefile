NAME	= pipex
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

MYLIB_DIR	= mylib
MYLIB		= $(MYLIB_DIR)/myLib.a

SRCS	=	src/pipex.c \
			src/childs.c \
			src/get_cmds.c \
			src/heredoc.c \
			src/ft_split_quote.c

OBJS	= $(SRCS:.c=.o)

all: $(MYLIB) $(NAME)
	@echo "Build complete!"

$(MYLIB):
	@echo "Building myLib"
	@$(MAKE) -s -C $(MYLIB_DIR)

%.o: %.c inc/pipex.h
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(MYLIB)
	@$(CC) $(CFLAGS) $(OBJS) $(MYLIB) -o $(NAME)

clean:
	@echo "Cleaning pipex object files"
	@rm -f $(OBJS)
	@$(MAKE) -s clean -C $(MYLIB_DIR)

fclean: clean
	@echo "Removing ./pipex"
	@rm -f $(NAME)
	@$(MAKE) -s fclean -C $(MYLIB_DIR)

re: fclean all

.PHONY: all clean fclean re
