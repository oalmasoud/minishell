NAME = minishell

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -I./includes -I$(LIBFT_DIR)
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline

RM = rm -f

SRC = 	srcs/main.c \
		srcs/utils/error_handling.c \
		srcs/utils/data_validation.c \
		srcs/utils/clean.c \
		srcs/environment_handling/pwd_dir.c \
		srcs/signals/signal_handler.c \
		srcs/tokenizer/process_input_command.c \
		srcs/integral/exit_integral.c \
		srcs/integral/cd_integral.c \
		srcs/integral/echo_integral.c \
		srcs/integral/env_integral.c \
		srcs/integral/export_integral.c \
		srcs/integral/pwd_integral.c \
		srcs/integral/unset_integral.c \
		srcs/tokenizer/tokenizer_utils.c \
		srcs/tokenizer/tokens_handler.c \
		srcs/tokenizer/tokens_handler_2.c \
		srcs/tokenizer/quotes.c \
		srcs/var/var_checker.c \
		srcs/var/var_value.c \
		srcs/var/var_value_2.c \
		srcs/var/var_utils.c \
		srcs/var/var_utils_2.c \
		srcs/commands/co_build.c \
		srcs/commands/co_build_2.c \
		srcs/commands/co_split.c \
		srcs/commands/co_word.c \
		srcs/commands/commands_utils.c \
		srcs/commands/commands.c \
		srcs/commands/co_input.c \
		srcs/commands/co_trunc.c \
		srcs/commands/co_heredoc.c \
		srcs/commands/co_heredoc_2.c \
		srcs/commands/co_heredoc_utils.c \
		srcs/commands/co_heredoc_utils_2.c \
		srcs/commands/co_append.c \
		srcs/commands/co_pipe.c \
		srcs/execution/shell_execution.c \
		srcs/execution/commands_execution.c \
		srcs/execution/shell_execution_utils.c \
		srcs/execution/execute.c \
		srcs/execution/execute_2.c \
		srcs/execution/cmd_path.c \
		
		
		
		
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): libft $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

libft:
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re libft
