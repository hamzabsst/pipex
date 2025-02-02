NAME = pipex
CC = cc
CFLAGS = -Wall -Werror -Wextra

MYLIB_DIR = mylib
MYLIB = $(MYLIB_DIR)/myLib.a

SRCS =	src/main.c \
		src/pipex.c \
		src/get_commands.c \

SRCS_BONUS =	bonus/get_cmds_bonus.c \
				bonus/here_doc.c \
				bonus/heredoc_ultils_bonus.c \
				bonus/pipex_bonus.c \
				bonus/utils_bonus.c \
				bonus/utils1_bonus.c


OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

NO_RELINK = .no_relink

all: $(MYLIB) $(NAME)

$(MYLIB):
		@$(MAKE) -C $(MYLIB_DIR)

%.o: %.c
		@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(MYLIB)
		@$(CC) $(CFLAGS) $(OBJS) $(MYLIB) -o $(NAME)

bonus: ${NO_RELINK}

${NO_RELINK}: $(OBJS_BONUS) $(MYLIB)
			@$(CC) $(CFLAGS) $(OBJS_BONUS) $(MYLIB) -o $(NAME)
			@touch ${NO_RELINK}

clean:
		@rm -f $(OBJS) $(OBJS_BONUS) $(NO_RELINK)
		@$(MAKE) clean -C $(MYLIB_DIR)

fclean: clean
		@rm -f $(NAME)
		@$(MAKE) fclean -C $(MYLIB_DIR)

re: fclean all

.PHONY: all clean fclean re bonus
